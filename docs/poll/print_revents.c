
#include <poll.h>
#include <stdio.h>

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