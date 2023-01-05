#pragma once

#include "ptt-config.h"

#if HAVE_GPIOD

#include "ptt.h"
#include "ptt-device.h"
#include <stdbool.h>

bool ptt_gpio_recognize(const char *device_name);
ptt_device_t *ptt_gpio_create(const char *device_name);

#endif // HAVE_GPIOD
