#pragma once

#include "ptt.h"
#include "ptt-button.h"

typedef struct ptt_button_list_s {
  ptt_button_t *head;
} ptt_button_list_t;

// device list operations
ptt_button_t *ptt_button_list_begin(ptt_button_list_t *list);
ptt_button_t *ptt_button_list_end(ptt_button_list_t *list);
void ptt_button_list_push_front(ptt_button_list_t *list, ptt_button_t *item);
void ptt_button_list_remove(ptt_button_list_t *list, ptt_button_t *item);
ptt_button_t *ptt_button_list_find(ptt_button_list_t *list, int id);
