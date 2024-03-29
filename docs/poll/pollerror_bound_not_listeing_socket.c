#include <arpa/inet.h> // Include for inet_addr
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void print_revents(short revents);

void set_non_blocking(int sockfd) {
  int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags == -1) {
    perror("fcntl");
    return;
  }
  flags |= O_NONBLOCK;
  if (fcntl(sockfd, F_SETFL, flags) == -1) {
    perror("fcntl");
    return;
  }
}

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("socket");
    return 1;
  }

  set_non_blocking(sockfd);

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(9999); // Try connecting to a local port that is
                               // unlikely to be in use or listening
  addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

  connect(sockfd, (struct sockaddr *)&addr,
          sizeof(addr)); // This is non-blocking

  struct pollfd pfd;
  pfd.fd = sockfd;
  pfd.events = POLLOUT; // Wait for write readiness indicating connect
                        // completion or error
  pfd.revents = 0;

  int ret = poll(&pfd, 1, 5000); // 5-second timeout
  printf("poll() returned %d\n", ret);

  if (ret > 0) {
    print_revents(pfd.revents);
    if (pfd.revents & POLLERR) {
      printf("POLLERR detected.\n");

      int error = 0;
      socklen_t errlen = sizeof(error);
      if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &errlen) == -1) {
        perror("getsockopt");
      } else {
        if (error == 0) {
          printf("Connection successful (unexpected).\n");
        } else {
          printf("Connection attempt failed: %s\n", strerror(error));
        }
      }
    } else if (pfd.revents & POLLOUT) {
      printf("POLLOUT received, connection might be successful or pending "
             "error check.\n");
    }
  } else if (ret == 0) {
    printf("Timeout occurred! No response after 5 seconds.\n");
  } else {
    perror("poll");
  }

  close(sockfd);
  return 0;
}
