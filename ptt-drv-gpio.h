#pragma once

#include "ptt.h"
#include "ptt-device.h"
#include <stdbool.h>

bool ptt_gpio_recognize(const char *device_name);
ptt_device_t *ptt_gpio_create(ptt_device_t *device, const char *device_name);
