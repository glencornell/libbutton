#pragma once

#include "ptt.h"
#include "ptt-key-list.h"
#include <stdbool.h>

typedef bool (*ptt_device_recognize_proc_t)(ptt_device_t *device, const char *device_name);
typedef ptt_device_t *(*ptt_device_create_proc_t)(ptt_device_t *device, const char *device_name);
typedef ptt_key_t *(*ptt_device_create_key_proc_t)(ptt_device_t *device, int key_number);
typedef void (*ptt_device_loop_iter_proc_t)(ptt_device_t *device);
typedef void (*ptt_device_destroy_proc_t)(ptt_device_t *device);
typedef bool (*ptt_device_has_fd_proc_t)(ptt_device_t *device);
typedef int (*ptt_device_get_fd_proc_t)(ptt_device_t *device);

typedef struct ptt_device_s {
  // name of the device
  char const *name;

  // virtual destructor
  ptt_device_destroy_proc_t destroy;

  // common methods
  ptt_device_create_key_proc_t create_key;
  ptt_device_loop_iter_proc_t loop_iter;
  ptt_device_has_fd_proc_t has_fd;
  ptt_device_get_fd_proc_t get_fd;

  // driver-specific implementation
  void *driver;

  // lists
  ptt_key_list_t keys;
  ptt_device_t *next;
} ptt_device_t;

bool ptt_device_has_fd(ptt_device_t *device);
int ptt_device_get_fd(ptt_device_t *device);
int ptt_device_readn(int fd, void *buf, ssize_t n);
bool ptt_device_file_exists(const char *path);
