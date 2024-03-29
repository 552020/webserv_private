#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_revents(short revents);

int main(void) {
  struct pollfd fds[2];
  int ret;

  // Open a temporary file to have a valid file descriptor.
  FILE *temp = tmpfile();
  if (temp == NULL) {
    perror("tmpfile");
    exit(EXIT_FAILURE);
  }
  // retrieve the file descriptor of the temp file
  int temp_fd = fileno(temp);

  // File descriptor 0 (stdin) should be valid.
  fds[0].fd = temp_fd;
  fds[0].events = POLLIN;

  // Intentionally invalid file descriptor to trigger POLLNVAL.
  fds[1].fd = -1; // Invalid file descriptor.
  fds[1].events = POLLIN;

  // Poll with a timeout of 5000 milliseconds (5 seconds).
  ret = poll(fds, 2, 5000);

  if (ret == -1) {
    print_revents(fds[0].revents);
    print_revents(fds[1].revents);
    perror("poll");
    exit(EXIT_FAILURE);
  } else if (ret == 0) {
    print_revents(fds[0].revents);
    print_revents(fds[1].revents);
    printf("No events occurred within the timeout period.\n");
  } else {
    print_revents(fds[0].revents);
    print_revents(fds[1].revents);
    if (fds[0].revents & POLLIN) {
      printf("Data is ready to be read from the temporary file descriptor.\n");
    }
    if (fds[1].revents & POLLNVAL) {
      printf("POLLNVAL event detected on the intentionally invalid file "
             "descriptor.\n");
    }
  }

  // Clean up.
  if (temp) {
    fclose(temp);
  }

  return 0;
}
