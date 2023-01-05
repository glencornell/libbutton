ACLOCAL_AMFLAGS=-I m4

lib_LTLIBRARIES = libbutton.la

if HAVE_EVDEV
EVDEV_SOURCES=src/ptt-drv-evdev.c src/ptt-drv-evdev.h
else
EVDEV_SOURCES=
endif

if HAVE_GPIOD
GPIOD_SOURCES=src/ptt-drv-gpio.c src/ptt-drv-gpio.h
else
GPIOD_SOURCES=
endif

libbutton_la_CFLAGS=$(all_includes)
libbutton_la_LDFLAGS=$(all_libraries)
libbutton_la_SOURCES = \
	src/ptt-button.h \
	src/ptt-button.c \
	src/ptt-button-list.c \
	src/ptt-button-list.h \
	src/ptt.c \
	src/ptt-device.c \
	src/ptt-device.h \
	src/ptt-device-list.c \
	src/ptt-device-list.h \
	$(EVDEV_SOURCES) \
	$(GPIOD_SOURCES) \
	src/ptt-util.c \
	src/ptt-util.h \
	src/ptt.h
include_HEADERS = \
	src/ptt.h
