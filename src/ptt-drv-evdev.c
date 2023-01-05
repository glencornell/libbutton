#include "ptt-config.h"
#include "ptt.h"
#include "ptt-device.h"
#include "ptt-drv-evdev.h"
#include "ptt-util.h"
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

static void ptt_evdev_destroy(ptt_device_t *device);
static void ptt_evdev_loop_iter(ptt_device_t *device);
static ptt_button_t *ptt_evdev_button_create(ptt_device_t *device, int keycode);
static void ptt_evdev_button_destroy(ptt_button_t *button);

bool ptt_evdev_recognize(const char *device_name) {
  static const char fmt[] = "/dev/input/";
  return (strncmp(device_name, fmt, strlen(fmt)) == 0) &&
    ptt_util_file_exists(device_name);
}

ptt_device_t *ptt_evdev_create(const char *name) {
  ptt_device_t *device = ptt_device_class_constructor(name);
  if (device) {
    ptt_evdev_t *driver = (ptt_evdev_t *)malloc(sizeof(ptt_evdev_t));
    if (driver) {
      if ((driver->fd = open(name, O_RDONLY)) != -1) {
        device->driver = (void*)driver;
        device->loop_iter = ptt_evdev_loop_iter;
        device->destroy = &ptt_evdev_destroy;
      } else {
        fprintf(stderr, "Cannot open %s: %s.\n", name, strerror(errno));
        ptt_device_class_destructor(device);
        device = NULL;
        free(driver);
      }
    } else {
      fprintf(stderr, "Cannot malloc %s: %s.\n", name, strerror(errno));
        ptt_device_class_destructor(device);
        device = NULL;
    }
  } else {
    fprintf(stderr, "Cannot create base class %s: %s.\n", name, strerror(errno));
  }
  return device;
}

static void ptt_evdev_destroy(ptt_device_t *device) {
  ptt_evdev_t *driver = device->driver;
  ptt_button_list_destructor(&device->buttons);
  if (driver) {
    close(driver->fd);
    driver->fd = -1;
    free(driver);
  }
  device->driver = NULL;
  device->loop_iter = NULL;
  device->destroy = NULL;
  ptt_device_destroy(device);
}

// Several keys may be connected to the same device.  Therefore, when
// we read from the device, we have to check if the read keycode
// matches any keys on this device that we are interested in.
static void ptt_evdev_loop_iter(ptt_device_t *device) {
  ptt_evdev_t *p = (ptt_evdev_t *)device->driver;
  struct input_event ev;
  ssize_t n;
  fd_set fds;
  ptt_button_t *button;
  
  FD_ZERO(&fds);
  FD_SET(p->fd, &fds);
  if (select(p->fd + 1, &fds, NULL, NULL, NULL) > 0) {
    if (FD_ISSET(p->fd, &fds)) {
      if (ptt_util_readn(p->fd, &ev, sizeof(ev)) == (ssize_t)-1) {
        // There's an error here
        fprintf(stderr, "Cannot read %s: %s.\n", device->name, strerror(errno));
        // TODO: how do we handle an error here?
        return;
      }
      // iterate through all keys listening on this descriptor
      for (button = ptt_button_list_begin(&device->buttons);
           button != ptt_button_list_end(&device->buttons);
           button = button->next) {
        if (ev.type == EV_KEY && ev.code == button->id) {
          switch(ev.value) {
          case KEY_PRESSED:
            printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
            button->button_state = PTT_BUTTON_STATE_PRESSED;
            if (button->prev_button_state != PTT_BUTTON_STATE_PRESSED) {
              button->prev_button_state = PTT_BUTTON_STATE_PRESSED;
              if (button->pressed_cb) button->pressed_cb(button, button->pressed_user_data);
            }
            break;
          case KEY_RELEASED:
            printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
            button->button_state = PTT_BUTTON_STATE_RELEASED;
            if (button->prev_button_state != PTT_BUTTON_STATE_RELEASED) {
              button->prev_button_state = PTT_BUTTON_STATE_RELEASED;
              if (button->released_cb) button->released_cb(button, button->released_user_data);
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

static ptt_button_t *ptt_evdev_button_create(ptt_device_t *device, int id) {
  ptt_button_t *button = ptt_button_class_constructor(device, id);
}
