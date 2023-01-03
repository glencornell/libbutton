#include "ptt.h"
#include "ptt-device.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

bool ptt_device_file_exists(const char *path) {
  struct stat s;
  return stat(path, &s) == 0;
}

bool ptt_device_has_fd(ptt_device_t *) {
  return false;
}

int ptt_device_get_fd(ptt_device_t *) {
  return -1;
}

// W. Richard Stevens was the greatest teacher!  Read n bytes from fd
// into buf.  This will continue reading until all bytes have been
// read.
int ptt_device_readn(int fd, void *buf, ssize_t n) {
  int bytes_read;
  while (n > 0) {
    if (bytes_read = read(fd, buf, n) == (ssize_t)-1) {
      if (errno == EINTR) {
        // no error here, the process was interrupted by a signal
        continue;
      } else {
        // Error here. return
        return -1;
      }
    }
    n -= bytes_read;
    buf += bytes_read;
  }
}

