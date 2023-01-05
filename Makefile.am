ACLOCAL_AMFLAGS=-I m4

lib_LTLIBRARIES = libbutton.la

if HAVE_EVDEV
EVDEV_SOURCES=ptt-drv-evdev.c ptt-drv-evdev.h
else
EVDEV_SOURCES=
endif

if HAVE_GPIOD
GPIOD_SOURCES=ptt-drv-gpio.c ptt-drv-gpio.h
else
GPIOD_SOURCES=
endif

libbutton_la_CFLAGS=$(all_includes)
libbutton_la_LDFLAGS=$(all_libraries)
libbutton_la_SOURCES = \
	ptt-button.h \
	ptt-button.c \
	ptt-button-list.c \
	ptt-button-list.h \
	ptt.c \
	ptt-device.c \
	ptt-device.h \
	ptt-device-list.c \
	ptt-device-list.h \
	$(EVDEV_SOURCES) \
	$(GPIOD_SOURCES) \
	ptt-util.c \
	ptt-util.h \
	ptt.h
include_HEADERS = \
	ptt.h
