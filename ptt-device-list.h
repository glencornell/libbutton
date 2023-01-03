#pragma once

#include "ptt.h"
#include "ptt-private.h"

// The ptt device list structure is opaque
struct ptt_device_list_s;
typedef struct ptt_device_list_s ptt_device_list_t;

// device list operations
ptt_device_list_t *ptt_device_list_get();
ptt_device_t *ptt_device_list_begin(ptt_device_list_t *list);
ptt_device_t *ptt_device_list_end(ptt_device_list_t *list);
void ptt_device_list_push_front(ptt_device_list_t *list, ptt_device_t *item);
void ptt_device_list_remove(ptt_device_list_t *list, ptt_device_t *item);
ptt_device_t *ptt_device_list_find(ptt_device_list_t *list, char *device_name);
