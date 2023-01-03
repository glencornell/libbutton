#include "ptt.h"
#include "ptt-device.h"
#include "ptt-dev-gpio.h"
#include <string.h>

static void ptt_gpio_destroy(ptt_device_t *device);
static bool ptt_gpio_is_pressed(ptt_device_t *device);

// driver-specific state:
typedef struct {
  struct gpiod_chip *chip;
} ptt_gpio_device_t;

typedef struct {
  struct gpiod_line *line;
} ptt_gpio_key_t;  

bool ptt_gpio_recognize(const char *device_name) {
  static const char fmt[] = "/dev/gpiochip";
  return (strncmp(device_name, fmt, strlen(fmt)) == 0) &&
    ptt_file_exists(device_name);
}

ptt_device_t *ptt_gpio_device_create(ptt_device_t *device, char const *device_name) {
  ptt_gpio_device_t *p = (ptt_gpio_device_t *)malloc(sizeof(ptt_gpio_device_t));
  device->driver = (void *) p;
  device->destroy_proc = &ptt_gpio_device_destroy;
  p->chip = gpiod_chip_open(device_name);
  return device;
}

ptt_key_t *ptt_gpio_key_create(ptt_device_t *device, ptt_key_t *key, int id) {
  ptt_gpio_device_t *driver = (ptt_gpio_device_t *)device->driver;
  ptt_gpio_key_t *p = (ptt_gpio_key_t *)malloc(sizeof(ptt_gpio_key_t));
  key->driver = (void *) p;
  key->destroy_proc = &ptt_gpio_key_destroy;
  key->is_pressed_proc = &ptt_gpio_key_is_pressed;
  p->line = gpiod_chip_get_line(p->chip, id);
  gpiod_line_request_input(p->line, "PTT");
  return device;
}

static void ptt_gpio_device_destroy(ptt_device_t *device) {
  ptt_gpio_device_t *p = (ptt_gpio_device_t *)device->driver;
  gpiod_chip_close(p->chip);
}

static void ptt_gpio_key_destroy(ptt_key_t *key) {
  ptt_gpio_key_t *p = (ptt_gpio_key_t *)key->driver;
  gpiod_line_release(p->line);
}

static bool ptt_gpio_key_is_pressed(ptt_device_t *key) {
  ptt_gpio_key_t *p = (ptt_gpio_key_t *)key->driver;
  int val = gpiod_line_get_value(p->line);
  return (val == 0 || val == -1) ? false : true;
}
