#include "ptt.h"
#include "ptt-device.h"
#include "ptt-device-list.h"
#include <stdlib.h>

ptt_device_t *ptt_device_class_constructor(const char *name) {
  ptt_device_t *device = (ptt_device_t *)malloc(sizeof(ptt_device_t));
  if (device) {
    device->name = name;
    device->destroy = &ptt_device_class_destructor;
    device->create_button = &ptt_device_class_create_button;
    device->loop_iter = &ptt_device_class_loop_iter;
    device->has_fd = &ptt_device_class_has_fd;
    device->get_fd = &ptt_device_class_get_fd;
    device->driver = NULL;
    ptt_button_list_constructor(&device->buttons);
    device->next = NULL;
    ptt_device_list_push_front(ptt_device_list_get(),device);
  }
  return device;
}

void ptt_device_destructor(ptt_device_t *device) {
  ptt_device_list_remove(ptt_device_list_get(), device);
  device->name = NULL;
  device->destroy = NULL;
  device->create_button = NULL;
  device->loop_iter = NULL;
  device->has_fd = NULL;
  device->get_fd = NULL;
  device->driver = NULL;
  device->next = NULL;
  ptt_button_list_destructor(&device->buttons);
  free(device);
}

bool ptt_device_class_has_fd(ptt_device_t *) {
  return false;
}

int ptt_device_class_get_fd(ptt_device_t *) {
  return -1;
}

ptt_button_t *ptt_device_class_create_button(ptt_device_t *, int) {
  return NULL;
}

void ptt_device_class_loop_iter(ptt_device_t *) {
}
