#pragma once

#include "ptt.h"
#include "ptt-device.h"
#include <stdbool.h>

bool ptt_evdev_recognize(const char *device_name);
ptt_device_t *ptt_evdev_create(ptt_device_t *device, const char *device_name);
