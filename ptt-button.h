#pragma once

#include "ptt.h"
#include <stdbool.h>

typedef enum {
  PTT_BUTTON_STATE_RELEASED = 0,
  PTT_BUTTON_STATE_PRESSED = 1,
  PTT_BUTTON_STATE_UNKNOWN = -1
} ptt_button_state_t;

// The button virtual destructor.  Each subclass is responsible for
// deallocating any memory that it creates and invoking the base
// button destructor, ptt_button_destructor().
typedef void (*ptt_button_destroy_proc_t)(ptt_button_t *button);

// The button virtual is_pressed method.  Each subclass will use this
// to read and dispatch button press and release events.
typedef bool (*ptt_button_is_pressed_proc_t)(ptt_button_t *button);

typedef struct ptt_button_s {
  // virtual destructor:
  ptt_button_destroy_proc_t destroy;

  // internal button state:
  int id;  // aka keycode, pin number
  ptt_device_t *device;  // the owner of this button
  void *driver; // derived class pointer
  ptt_button_state_t prev_button_state;
  ptt_button_state_t button_state;

  // method to read button state from device:
  ptt_button_is_pressed_proc_t is_pressed;

  // pressed callback
  ptt_button_pressed_cb_t pressed_cb;
  void *pressed_user_data;

  // released callback
  ptt_button_released_cb_t released_cb;
  void *released_user_data;

  // list
  ptt_button_t *next;
} ptt_button_t;  

// Common base class methods that the driver can override/call:
ptt_button_t *ptt_button_class_constructor(ptt_device_t *device, int id);
void ptt_button_class_destructor(ptt_button_t *button);
bool ptt_button_class_is_pressed(ptt_button_t *button);
