#pragma once

#include <stdbool.h>
#include <unistd.h>

// read n bytes from a file descriptor. This will continue if a signal
// interrupts the read.
int ptt_util_readn(int fd, void *buf, ssize_t n);

// Returns true if a path exists
bool ptt_util_file_exists(const char *path);
