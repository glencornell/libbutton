# libbutton

A simple library for reading various types of momentary push buttons.

## Motivation

This library came out of a need to read key press and release events
from various sources for my GNU/Linux projects.

This library reads from [evdev](https://en.wikipedia.org/wiki/Evdev)
devices (those found at `/dev/input`), such as keyboards, mice and
joysticks.  Furthermore, this library reads GPIO signals using
`libgpiod`.  So it should work pretty well on a Raspberry Pi or a
BeagleBone.

## Dependencies

This library depends upon the following packages:

```bash
sudo apt install libgpiod-dev
```

## Compiling

This library is configured and built with GNU autotools. To build the
library, type the following:

```bash
autoreconf -vi
./configure
make
make install
```

## Using

To compile and link your application with this library, user
pkg-config:

```bash
gcc -o test test.c $(pkg-config --cflags --libs libbutton)
```

## TODO

Things I need to finish:

- [ ] Remove pthreads from the evdev driver
- [ ] Better integration with existing event loops (libevent2, libuv, etc.)
- [ ] Test
