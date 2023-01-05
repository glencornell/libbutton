#include "ptt-config.h"
#include "ptt.h"
#include "ptt-device.h"
#include "ptt-button.h"
#include "ptt-config.h"
#include "ptt-drv-evdev.h"
#include "ptt-drv-gpio.h"
#include <stdlib.h>

// Each implementation should have its own recognize() and create()
// functions.
typedef struct {
  ptt_device_recognize_proc_t recognize;
  ptt_device_create_proc_t create;
} ptt_constructor_table_t;

// The following table contains the recognize() and device_create()
// static class methods that each driver implements.  The
// ptt_device_create() function below iterates through this table to
// open the appropriate driver.
static const ptt_constructor_table_t ptt_constructor_table[] = {
#if HAVE_EVDEV
  {
    &ptt_evdev_recognize,
    &ptt_evdev_create
  },
#endif // HAVE_EVDEV
#if HAVE_GPIOD
  {
    &ptt_gpio_recognize,
    &ptt_gpio_create
  },
#endif // HAVE_GPIOD
  // TODO: create a null driver?
  // TODO: create a curses driver?
  // TODO: create an X11 driver?
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
      device = ptt_constructor_table[i].create(device_name);
      break;
    }
  }
  return device;
}

void ptt_device_destroy(ptt_device_t *device) {
  device->destroy(device);
}

bool ptt_button_is_pressed(ptt_button_t *button) {
  return button->is_pressed(button);
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

