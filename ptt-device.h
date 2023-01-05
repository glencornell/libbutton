#pragma once

#include "ptt.h"
#include "ptt-button-list.h"
#include <stdbool.h>

// This static class method is implemented by the driver.  It is
// called once by ptt_device_create (or ptt_device_recgnize() by the
// user) to determine if the device name is recognized by the driver.
// For example, if the user supplies a pathname of
// "/dev/input/event13", the evdev driver will return true.
typedef bool (*ptt_device_recognize_proc_t)(const char *device_name);

// Each driver needs to implement a device create procedure.  This is
// called by ptt_device_create().
typedef ptt_device_t *(*ptt_device_create_proc_t)(const char *device_name);

// Each driver implements a driver-specific button constructor, which
// extends the ptt_button_t struct.
typedef ptt_button_t *(*ptt_device_create_button_proc_t)(ptt_device_t *device, int id);

// main loop iterator.  Each driver may have a unique way of reading
// button state and dispatching the appropriate callbacks.
typedef void (*ptt_device_loop_iter_proc_t)(ptt_device_t *device);

// class destructor.  Each subclass is responsible for calling the
// base class destructor, ptt_device_destructor() and deallocating any
// memory that it allocated.  The base class destructor will
// deallocate any memory that it created.
typedef void (*ptt_device_destroy_proc_t)(ptt_device_t *device);

// Does the driver use file descriptors?
typedef bool (*ptt_device_has_fd_proc_t)(ptt_device_t *device);

// Return the file descriptor for this device, if avialable. returns
// -1 otherwise.
typedef int (*ptt_device_get_fd_proc_t)(ptt_device_t *device);


typedef struct ptt_device_s {
  // name of the device
  char const *name;

  // virtual destructor
  ptt_device_destroy_proc_t destroy;

  // common methods
  ptt_device_create_button_proc_t create_button;
  ptt_device_loop_iter_proc_t loop_iter;
  ptt_device_has_fd_proc_t has_fd;
  ptt_device_get_fd_proc_t get_fd;

  // driver-specific implementation
  void *driver;

  // lists
  ptt_button_list_t buttons;
  ptt_device_t *next;
} ptt_device_t;

// The base class constructor should be the first thing that the
// derived driver classes call.  This base class constructor will
// allocate memory for the base class, then assign default virtual
// methods: has_fd() returns false, get_fd() returns -1, and all other
// methods do nothing.  The driver's constructor will override this
// behavior.
ptt_device_t *ptt_device_class_constructor(const char *name);

// This will deallocate any memory allocated in the base class
// constructor.  It should therefore be the last thing called in the
// derived driver's destructor.  This method will NEITHER remove NOR
// destroy any buttons from its list.  That should be performed by the
// derived driver's destructor.
void ptt_device_class_destructor(ptt_device_t *device);

// always returns false. Drivers are expected to override this,unless
// that's the behavior it needs.
bool ptt_device_class_has_fd(ptt_device_t *device);

// always returns -1;
int ptt_device_class_get_fd(ptt_device_t *device);

// does nothing. derived classes override this.
ptt_button_t *ptt_device_class_create_button(ptt_device_t *device, int id);

// does nothing. derived classes override this.
void ptt_device_class_loop_iter(ptt_device_t *);
