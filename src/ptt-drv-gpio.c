#include "ptt-config.h"
#include "ptt.h"
#include "ptt-device.h"
#include "ptt-drv-gpio.h"
#include "ptt-util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <gpiod.h>

static void ptt_gpio_device_destroy(ptt_device_t *device);
static void ptt_gpio_button_destroy(ptt_button_t *button);
static bool ptt_gpio_button_is_pressed(ptt_button_t *button);

// driver-specific state:
typedef struct {
  struct gpiod_chip *chip;
} ptt_gpio_device_t;

typedef struct {
  struct gpiod_line *line;
} ptt_gpio_button_t;

bool ptt_gpio_recognize(const char *device_name) {
  static const char fmt[] = "/dev/gpiochip";
  return (strncmp(device_name, fmt, strlen(fmt)) == 0) &&
    ptt_util_file_exists(device_name);
}

ptt_device_t *ptt_gpio_device_create(char const *name) {
  ptt_device_t *device = ptt_device_class_constructor(name);
  if (device) {
    ptt_gpio_device_t *driver = (ptt_gpio_device_t *)malloc(sizeof(ptt_gpio_device_t));
    if (driver) {
      if (driver->chip = gpiod_chip_open(name)) {
        device->driver = (void*)driver;
        device->destroy = &ptt_gpio_device_destroy;
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

ptt_button_t *ptt_gpio_key_create(ptt_device_t *device, int id) {
  ptt_button_t *button = ptt_button_class_constructor(device, id);
  if (button) {
    ptt_gpio_button_t *gpio_button = (ptt_gpio_button_t *)malloc(sizeof(ptt_gpio_button_t));
    if (gpio_button) {
      ptt_gpio_device_t *gpio_device = (ptt_gpio_device_t *)device->driver;
      button->destroy = &ptt_gpio_button_destroy;
      button->is_pressed = &ptt_gpio_button_is_pressed;
      gpio_button->line = gpiod_chip_get_line(gpio_device->chip, id);
      gpiod_line_request_input(gpio_button->line, "ptt");
    } else {
      fprintf(stderr, "Cannot malloc button for device %s: %s.\n",
              device->name, strerror(errno));
        ptt_button_class_destructor(button);
        device = NULL;
    }
  } else {
    fprintf(stderr, "Cannot create button base class for device %s: %s.\n",
            device->name, strerror(errno));
  }
  return button;
}

static void ptt_gpio_device_destroy(ptt_device_t *device) {
  ptt_gpio_device_t *driver = (ptt_gpio_device_t *)device->driver;
  gpiod_chip_close(driver->chip);
  ptt_device_class_destructor(device);
}

static void ptt_gpio_button_destroy(ptt_button_t *button) {
  ptt_gpio_button_t *gpio_button = (ptt_gpio_button_t *)button->driver;
  gpiod_line_release(gpio_button->line);
  free(gpio_button);
  button->driver = NULL;
  ptt_button_class_destructor(button);
}

static bool ptt_gpio_button_is_pressed(ptt_button_t *button) {
  ptt_gpio_button_t *gpio_button = (ptt_gpio_button_t *)button->driver;
  int val = gpiod_line_get_value(gpio_button->line);
  return (val == 0 || val == -1) ? false : true;
}
