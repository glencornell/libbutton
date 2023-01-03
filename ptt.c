#include "ptt.h"
#include "ptt-device.h"
#include "ptt-button.h"
#include "ptt-config.h"
#include "ptt-evdev.h"
#include "ptt-gpio.h"

// Each implementation should have its own recognize() and create()
// functions.
typedef struct {
  ptt_device_recognize_proc_t recognize;
  ptt_device_create_proc_t create;
} ptt_table_t;

static const ptt_constructor_table_t ptt_constructor_table[] = {
#if HAVE_EVDEV
  {
    &ptt_evdev_recognize_proc,
    &ptt_evdev_create_proc
  },
#endif // HAVE_EVDEV
#if HAVE_GPIO
  {
    &ptt_gpio_recognize_proc,
    &ptt_gpio_create_proc
  },
#endif // HAVE_GPIO
};

bool ptt_device_recognize(const char *device_name) {
  int i;
  bool rval = false;
  const int table_size = sizeof(ptt_constructor_table) / sizeof(ptt_constructor_table_t);
  for (i = 0; i < table_size; i++) {
    if (ptt_constructor_table[i].recognize(device_name)) {
      rval = true;
      break;
    }
  }
  return rval;
}

ptt_device_t *ptt_device_create(const char *device_name) {
  ptt_device_t *device = NULL;
  int i;
  const int table_size = sizeof(ptt_constructor_table) / sizeof(ptt_constructor_table_t);
  for (i = 0; i < table_size; i++) {
    if (ptt_constructor_table[i].recognize(device_name)) {
      device = (ptt_device_t *)malloc(sizeof(ptt_device_t));
      device->fd = -1;
      device->destroy = NULL;
      device->has_fd = &ptt_priv_has_fd;
      device->get_fd = &ptt_priv_get_fd;
      if (ptt_constructor_table[i].create(device, device_name) == NULL) {
        free(device);
        device = NULL;
      }
      break;
    }
  }
  return device;
}

void ptt_device_destroy(ptt_device_t *device) {
  // TODO: iterate through all keys and deallocate
  device->destroy_proc(device);
  
  device->fd = -1;
  device->destroy = NULL;
  device->has_fd = NULL;
  device->get_fd = NULL;
  free(device);
}

bool ptt_button_is_pressed(ptt_button_t *button) {
  return button->is_pressed_proc(button);
}

void ptt_button_add_pressed_cb(ptt_button_t *button, ptt_button_pressed_cb_t cb, void *user_data) {
  button->pressed_cb = cb;
  button->pressed_user_data = user_data;
}

void ptt_button_add_released_cb(ptt_button_t *button, ptt_button_released_cb_t cb, void *user_data) {
  button->released_cb = cb;
  button->released_user_data = user_data;
}

bool ptt_device_has_fd(ptt_device_t *device) {
  return device->has_fd(device);
}

int ptt_device_get_fd(ptt_device_t *device) {
  return device->get_fd(device);
}

void ptt_device_loop_iter(ptt_device_t *device) {
  device->loop_iter(device);
}

