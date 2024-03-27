#include <poll.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  struct pollfd fds;
  int ret;

  // Set up the pollfd structure
  fds.fd = 0;          // File descriptor 0 (stdin)
  fds.events = POLLIN; // Monitor for input

  // Wait for up to 5000 milliseconds
  ret = poll(&fds, 1, 5000);

  if (ret == -1) {
    perror("poll");
    return 1;
  } else if (ret == 0) {
    printf("Timeout occurred! No input detected.\n");
  } else {
    if (fds.revents & POLLIN) {
      printf("Data is ready to be read.\n");
    }
  }

  return 0;
}
