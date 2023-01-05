#pragma once

#include "ptt-config.h"

#if HAVE_EVDEV

#include "ptt.h"
#include "ptt-device.h"
#include <stdbool.h>

bool ptt_evdev_recognize(const char *device_name);
ptt_device_t *ptt_evdev_create(const char *device_name);

#endif // HAVE_EVDEV
