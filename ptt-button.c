#include "ptt-config.h"
#include "ptt.h"
#include "ptt-device.h"
#include "ptt-button.h"
#include "ptt-button-list.h"
#include <stdlib.h>

ptt_button_t *ptt_button_class_constructor(ptt_device_t *device, int id) {
  ptt_button_t *button = (ptt_button_t *)malloc(sizeof(ptt_button_t));
  if(button) {
    button->destroy = &ptt_button_class_destructor;
    button->id = id;
    button->device = device;
    button->driver = NULL;
    button->prev_button_state = PTT_BUTTON_STATE_UNKNOWN;
    button->button_state = PTT_BUTTON_STATE_UNKNOWN;
    button->is_pressed = &ptt_button_class_is_pressed;
    button->pressed_cb = NULL;
    button->pressed_user_data = NULL;
    button->released_cb = NULL;
    button->released_user_data = NULL;
    button->next = NULL;
    ptt_button_list_push_front(&button->device->buttons, button);
  }
  return button;
}

void ptt_button_class_destructor(ptt_button_t *button) {
  ptt_button_list_remove(&button->device->buttons, button);
  button->destroy = NULL;
  button->id = -1;
  button->device = NULL;
  button->driver = NULL;
  button->prev_button_state = PTT_BUTTON_STATE_UNKNOWN;
  button->button_state = PTT_BUTTON_STATE_UNKNOWN;
  button->is_pressed = NULL;
  button->pressed_cb = NULL;
  button->pressed_user_data = NULL;
  button->released_cb = NULL;
  button->released_user_data = NULL;
  button->next = NULL;
  free(button);
}

bool ptt_button_class_is_pressed(ptt_button_t *button) {
  return button->button_state == PTT_BUTTON_STATE_PRESSED;
}

