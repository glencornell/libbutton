#pragma once

#include "ptt.h"
#include <stdbool.h>

typedef enum {
  PTT_BUTTON_STATE_RELEASED = 0,
  PTT_BUTTON_STATE_PRESSED = 1,
  PTT_BUTTON_STATE_UNKNOWN = -1
} ptt_button_state_t;

typedef void (*ptt_button_destroy_proc_t)(ptt_button_t *button);
typedef bool (*ptt_button_is_pressed_proc_t)(ptt_button_t *button);

typedef struct ptt_button_s {
  // virtual destructor:
  ptt_button_destroy_proc_t destroy;

  // internal button state:
  int id;  // aka keycode, pin number
  ptt_button_state_t prev_button_state;
  ptt_button_state_t button_state;

  // method to read button state from device:
  ptt_button_is_pressed_proc_t is_pressed;

  // pressed callback
  ptt_button_pressed_cb_t pressed_cb;
  void *pressed_user_data;

  // released callback
  ptt_released_cb_t released_cb;
  void *released_user_data;

  // list
  ptt_button_t *next;
} ptt_button_t;  
