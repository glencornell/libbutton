#pragma once

#include <stdbool.h>

// Here are some application defaults:
// - input source is the /dev/input linux driver
// - the left control button is the push-to-talk button
#define DEFAULT_PTT_INPUT_SOURCE PTT_INPUT_SOURCE_DEV_INPUT
#define DEFAULT_PTT_DEV_INPUT_KEYCODE KEY_LEFTCTRL
#define DEFAULT_PTT_DEV_INPUT_DEVICE "/dev/input/by-path/platform-i8042-serio-0-event-kbd"

// When the GPIO driver is selected, the default pin is 1
#define DEFAULT_PTT_GPIO_DEVICE "/dev/gpiochip0"
#define DEFAULT_PTT_GPIO_PIN 1

// This is the public API. Only use functions and data types
// identified in this file.

// This library consists of devices and buttons.  Devices may have one
// or more buttons attached.  You first create a device by its name,
// then attach buttons to the device by keycode.  The keycode is
// device-specific.


// The ptt device structure is opaque
struct ptt_device_s;
typedef struct ptt_device_s ptt_device_t;

// query to determine if the device is supported
bool ptt_device_recognize(const char *device_name);

// object constructor - this function will open the given device name
// and allocate memory for the object.  If device_name is NULL, then
// the library will try to open the default device for the system.
ptt_device_t *ptt_device_create(char const *device_name);

// object destructor - this will destroy all attached buttons
void ptt_device_destroy(ptt_device_t *device);

// invoke this from your main loop to invoke the button state change
// callbacks.  This is a non-blocking function; it will return
// immediately if no data is available to be read.
void ptt_device_loop_iter_all();
void ptt_device_loop_iter(ptt_device_t *device);

// does this interface have a file descriptor?
bool ptt_device_has_file_descriptor(ptt_device_t *device);

// return the file descriptor, if available. returns -1 otherwise.
int ptt_device_get_fd(ptt_device_t *device);

// start reading from the device
void ptt_device_start_all();
void ptt_device_start(ptt_device_t *device);


// The ptt button structure is opaque
struct ptt_button_s;
typedef struct ptt_button_s ptt_button_t;

// Callbacks that are invoked when the PTT button changes state
typedef void (*ptt_button_pressed_cb_t)(ptt_button_t *button, void *user_data);
typedef void (*ptt_button_released_cb_t)(ptt_button_t *button, void *user_data);

// object destructor
void ptt_button_destroy(ptt_button_t *button);

// Add button state change callbacks:
void ptt_button_add_pressed_cb(ptt_button_t *button, ptt_button_pressed_cb_t cb, void *user_data);
void ptt_button_add_released_cb(ptt_button_t *button, ptt_button_released_cb_t cb, void *user_data);

// get the current state of the button
bool ptt_button_is_pressed(ptt_button_t *button);
