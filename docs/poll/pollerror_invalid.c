#include <arpa/inet.h> // Include this header for inet_addr
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Trying to trigger a POLLERR event by connecting to an invalid address
// But it

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
  addr.sin_port = htons(9999); // Use an unlikely port number
  addr.sin_addr.s_addr =
      inet_addr("192.0.2.1"); // Use the TEST-NET-1 IP block, which is reserved
                              // for documentation

  connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));

  struct pollfd pfd;
  pfd.fd = sockfd;
  pfd.events =
      POLLOUT; // We're interested in the completion of the connection attempt

  int ret = poll(&pfd, 1, 5000); // Wait for 5 seconds

  if (ret > 0) {
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
          perror("Connection attempt failed");
        }
      }
    } else if (pfd.revents & POLLOUT) {
      // Normally, POLLOUT here means the connection attempt succeeded or is in
      // progress, but since we're connecting to an invalid address, this
      // outcome is unexpected.
      printf("POLLOUT received, but connection should not be successful.\n");
    }
  } else if (ret == 0) {
    printf("Timeout occurred! No response after 5 seconds.\n");
  } else {
    perror("poll");
  }

  close(sockfd);
  return 0;
}
