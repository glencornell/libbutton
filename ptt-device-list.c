#include "ptt.h"
#include "ptt-private.h"
#include "ptt-list.h"

typedef struct ptt_device_list_s {
  ptt_device_t *head;
} ptt_device_list_t;

static ptt_device_list_t devices = {
  .head = NULL
};

ptt_device_list_t *ptt_device_list_get() {
  return keys;
}

ptt_device_t *ptt_device_list_begin(ptt_device_list_t *list) {
  return list->head;
}

ptt_device_t *ptt_device_list_end(ptt_device_list_t *) {
  return NULL;
}

void ptt_device_list_push_front(ptt_device_list_t *list, ptt_device_t *item) {
  item->next = list->head;
  list->head = item;
}

ptt_device_t *ptt_device_list_find(ptt_device_list_t *list, char *device_name) {
  ptt_device_t *i;
  for (i = ptt_device_list_begin(list); i != ptt_device_list_end(list); i = i->next) {
    if ((i->device == device_name) || (strcmp(i->device, device_name)==0)) {
      return i;
    }
  }
  return ptt_device_list_end(list);
}

void ptt_device_list_remove(ptt_device_list_t *list, ptt_device_t *item) {
  ptt_device_t *i;
  if (list->next == item) {
    list->next = item->next;
    item->next = NULL;
    return;
  }
  for (i = ptt_device_list_begin(list); i != ptt_device_list_end(list); i = i->next) {
    if (i->next == item) {
      i->next = item->next;
      item->next = NULL;
      return;
    }
  }
}
