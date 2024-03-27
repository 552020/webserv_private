#include <poll.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

void print_revents_binary(short revents) {
  for (int i = 15; i >= 0; i--) {
    putchar((revents & (1 << i)) ? '1' : '0');
    if (i % 4 == 0)
      putchar(' ');
  }
  putchar('\n');
}

void print_revents(short revents) {
  printf("revents = %d\n", revents);
  printf("revents binary = ");
  print_revents_binary(revents);

  if (revents & POLLIN)
    printf("POLLIN\n");
  if (revents & POLLPRI)
    printf("POLLPRI\n");
  if (revents & POLLOUT)
    printf("POLLOUT\n");
  if (revents & POLLERR)
    printf("POLLERR ");
  if (revents & POLLHUP)
    printf("POLLHUP ");
  if (revents & POLLNVAL)
    printf("POLLNVAL ");
  if (revents & POLLRDNORM)
    printf("POLLRDNORM\n");
  if (revents & POLLWRNORM)
    printf("POLLWRNORM\n");
  if (revents & POLLRDBAND)
    printf("POLLRDBAND\n");
  if (revents & POLLWRBAND)
    printf("POLLWRBAND\n");
  if (revents & POLLEXTEND)
    printf("POLLEXTEND\n");
  printf("\n");
}

int main() {
  int socket[2];
  struct pollfd pfd;

  // Create a pair of connected sockets
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, socket) == -1) {
    perror("socketpair");
    return 1;
  }

  // Close one end of the connection
  close(socket[1]);

  // Setup poll() to monitor the other socket
  pfd.fd = socket[0];
  pfd.events = POLLIN;
  pfd.revents = 0;

  // Call poll() with a timeout of 5000 milliseconds
  int ret = poll(&pfd, 1, 5000);

  if (ret > 0) {
    print_revents(pfd.revents);
    // Check if POLLHUP is reported
    if (pfd.revents & POLLHUP) {
      printf("POLLHUP received: the other side of the connection has been "
             "closed.\n");
      if (pfd.revents & POLLIN) {
        printf("Data is ready to be read.\n");
      }
    } else if (ret == 0) {
      printf("Timeout occurred! No data after 5000 milliseconds\n");
    } else {
      perror("poll");
    }
    close(socket[0]);
    return 0;
  }
}
