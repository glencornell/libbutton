#include "ptt.h"
#include "ptt-button.h"
#include "ptt-button-list.h"

ptt_button_t *ptt_button_list_begin(ptt_button_list_t *list) {
  return list->head;
}

ptt_button_t *ptt_button_list_end(ptt_button_list_t *) {
  return NULL;
}

void ptt_button_list_push_front(ptt_button_list_t *list, ptt_button_t *item) {
  item->next = list->head;
  list->head = item;
}

ptt_button_t *ptt_button_list_find(ptt_button_list_t *list, int id) {
  ptt_button_t *i;
  for (i = ptt_button_list_begin(list); i != ptt_button_list_end(list); i = i->next) {
    if (i->id == id) {
      return i;
    }
  }
  return ptt_button_list_end(list);
}

void ptt_button_list_remove(ptt_button_list_t *list, ptt_edev_t *item) {
  ptt_button_t *i;
  if (list->next == item) {
    list->next = item->next;
    item->next = NULL;
    return;
  }
  for (i = ptt_button_list_begin(list); i != ptt_button_list_end(list); i = i->next) {
    if (i->next == item) {
      i->next = item->next;
      item->next = NULL;
      return;
    }
  }
}
