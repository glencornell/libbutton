#include "ptt-util.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

bool ptt_util_file_exists(const char *path) {
  struct stat s;
  return stat(path, &s) == 0;
}

// W. Richard Stevens was the greatest teacher!  Read n bytes from fd
// into buf.  This will continue reading until all bytes have been
// read.
int ptt_util_readn(int fd, void *buf, ssize_t n) {
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

