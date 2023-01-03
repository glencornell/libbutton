#include "ptt.h"
#include "ptt-device.h"
#include "ptt-dev-evdev.h"
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define KEY_RELEASED 0
#define KEY_PRESSED  1
#define KEY_REPEATED 2

static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

typedef struct ptt_evdev_t {
  int fd;
} ptt_evdev_t;

static bool ptt_evdev_is_pressed(ptt_key_t *key);
static void ptt_evdev_destroy(ptt_key_t *key);
static void ptt_evdev_loop_iter(ptt_device_t *device);

bool ptt_evdev_recognize(const char *device_name) {
  static const char fmt[] = "/dev/input/";
  return (strncmp(device_name, fmt, strlen(fmt)) == 0) &&
    ptt_priv_file_exists(device_name);
}

ptt_device_t *ptt_evdev_create(ptt_device_t *device, const char *input_device) {
  ptt_device_t *dev = ptt_device_list_find(ptt_device_list_get(), input_device);
  
  if (dev == ptt_device_list_end(ptt_device_list_get())) {
    // There is no existing device with this name. open it.
    ptt_evdev_t *p = (ptt_evdev_t *)malloc(sizeof(ptt_evdev_t));

    if (p == NULL) {
      fprintf(stderr, "Cannot malloc %s: %s.\n", device->device, strerror(errno));
      return NULL;
    }

    device->device = input_device;
    if ((p->fd = open(device->device, O_RDONLY)) == -1) {
      free(p);
      fprintf(stderr, "Cannot open %s: %s.\n", device->device, strerror(errno));
      return NULL;
    }
    p->fd = fd;
  }

  ptt->device = input_device;
  ptt->driver_state = (void*)p;
  ptt->loop_iter_proc = ptt_evdev_loop_iter;
  ptt->destroy_proc = ptt_evdev_destroy;
  ptt->is_pressed_proc = ptt_evdev_is_pressed;
  return ptt;
}

static void ptt_evdev_destroy(ptt_t *ptt) {
  ptt_evdev_t *p = ptt->driver_state;
  if (p) {
    // remove this instance from the list
    ptt_list_remove(ptt_list_get(), p);
    if (ptt_list_find(ptt_list_get(), ptt->device) == ptt_list_end(ptt_list_get())) {
      // The device name is no longer being used. Close it
      close(p->fd);
      p->fd = -1;
      free(p);
    }
  }
  ptt->device = NULL;
  ptt->driver_state = NULL;
  ptt->loop_iter_proc = NULL;
  ptt->destroy_proc = NULL;
  ptt->is_pressed_proc = NULL;
}

static bool ptt_evdev_is_pressed(ptt_t *ptt) {
  return ptt->key_state == PTT_KEY_STATE_PRESSED;
}

// Several keys may be connected to the same device.  Therefore, when
// we read from the device, we have to check if the read keycode
// matches any keys on this device that we are interested in.
static void ptt_evdev_loop_iter(ptt_device_t *device) {
  ptt_evdev_t *p = (ptt_evdev_t *)device->driver_state;
  struct input_event ev;
  ssize_t n;
  fd_set fds;
  ptt_key_t *key;
  
  FD_ZERO(&fds);
  FD_SET(&fds, p->fd);
  if (select(p->fd + 1, &fds, NULL, NULL, NULL) > 0) {
    if (FD_ISSET(p->fd, &fds)) {
      if (ptt_priv_readn(p->fd, &ev, sizeof(ev)) == (ssize_t)-1) {
        // There's an error here
        fprintf(stderr, "Cannot read %s: %s.\n", device->device, strerror(errno));
        // TODO: do we shut down here?
        return;
      }
      // iterate through all keys listening on this descriptor
      for (key = ptt_key_list_begin(device->key_list); key != ptt_key_list_end(device->key_list); key = key->next) {
        if (ev.type == EV_KEY && ev.code == key->keycode) {
          switch(ev.value) {
          case KEY_PRESSED:
            printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
            key->key_state = PTT_KEY_STATE_PRESSED;
            if (key->prev_key_state != PTT_KEY_STATE_PRESSED) {
              key->prev_key_state = PTT_KEY_STATE_PRESSED;
              if (key->pressed_cb) key->pressed_cb(key, key->pressed_user_data);
            }
            break;
          case KEY_RELEASED:
            printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
            key->key_state = PTT_KEY_STATE_RELEASED;
            if (key->prev_key_state != PTT_KEY_STATE_RELEASED) {
              key->prev_key_state = PTT_KEY_STATE_RELEASED;
              if (key->released_cb) key->released_cb(key, key->released_user_data);
            }
            break;
          case KEY_REPEATED:
            break;
          }
        }
      }
    }
  }
}
