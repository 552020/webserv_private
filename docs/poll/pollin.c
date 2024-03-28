#include <fcntl.h>      // for fcntl
#include <netinet/in.h> // for htons, htonl,
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> // for socketpair
#include <unistd.h>

void print_revents(short revents);

void set_non_blocking(int sockfd) {
  int flags = fcntl(sockfd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(sockfd, F_SETFL, flags);
}

void test_network_socket_client_server_message() {
  int server_fd, client_fd;
  struct sockaddr_in server_addr;

  // Create and bind the server socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(0); // Let OS choose the port

  bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  socklen_t len = sizeof(server_addr);
  getsockname(server_fd, (struct sockaddr *)&server_addr,
              &len); // Get the port number

  listen(server_fd, 1);

  // Create the client socket and connect to the server
  client_fd = socket(AF_INET, SOCK_STREAM, 0);
  set_non_blocking(client_fd); // Make client socket non-blocking

  // Attempt to connect to the server
  connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  printf("Waiting for connection on dynamically assigned port...\n");

  // Server accepts connection
  struct pollfd server_pfd;
  server_pfd.fd = server_fd;
  server_pfd.events = POLLIN;

  if (poll(&server_pfd, 1, 5000) >
      0) { // 5-second timeout for accepting connection
    printf("Connection incoming\n");
    int new_sock = accept(server_fd, NULL, NULL);
    if (new_sock >= 0) {
      printf("Connection accepted\n");

      // Wait for message from client
      server_pfd.fd = new_sock; // Now wait for data on the new socket
      if (poll(&server_pfd, 1, 5000) > 0) { // 5-second timeout for data
        if (server_pfd.revents & POLLIN) {
          char buffer[1024] = {0};
          recv(new_sock, buffer, sizeof(buffer), 0);
          printf("Message from client: %s\n", buffer);
        }
      } else {
        printf("No data received within timeout period.\n");
      }

      close(new_sock);
    }
  } else {
    printf("No connection within timeout period.\n");
  }

  // Assume the client will send data immediately after connection
  // In a real scenario, you might wait or poll on client_fd for POLLOUT before
  // sending

  const char *msg = "Hello from client!";
  send(client_fd, msg, strlen(msg), 0);
  printf("Message sent to server.\n");

  close(client_fd);
  close(server_fd);
}

void test_network_socket_client_server() {
  int server_fd, client_fd;
  struct sockaddr_in server_addr, client_addr;

  // Create server socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(0); // Let OS choose the port

  bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  socklen_t len = sizeof(server_addr);
  getsockname(server_fd, (struct sockaddr *)&server_addr,
              &len); // Get the port number

  listen(server_fd, 1);

  // Create client socket
  client_fd = socket(AF_INET, SOCK_STREAM, 0);
  set_non_blocking(client_fd); // Make client socket non-blocking

  // Attempt to connect to the server
  connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  // Set up poll to monitor the server socket for incoming connections
  struct pollfd pfd;
  pfd.fd = server_fd;
  pfd.events = POLLIN;
  pfd.revents = 0;

  printf("Waiting for connection on port %d\n", ntohs(server_addr.sin_port));

  if (poll(&pfd, 1, 5000) > 0) { // 5-second timeout
    print_revents(pfd.revents);
    if (pfd.revents & POLLIN) {
      printf("Connection incoming\n");
      int new_sock = accept(server_fd, NULL, NULL);
      if (new_sock >= 0) {
        printf("Connection accepted\n");
        close(new_sock);
      }
    }
  } else {
    printf("No connection within timeout period.\n");
  }

  close(client_fd);
  close(server_fd);
}

void test_network_socket() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  // Port 9999 is an arbitrary choice: it is not a privileged port
  addr.sin_port = htons(9999);
  // Listen on all network interfaces
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
  listen(server_fd, 1);

  // In a real application, you'd need another process or thread to accept the
  // connection. For simplicity, we assume the connection will be established by
  // an external client.

  struct pollfd pfd;
  pfd.fd = server_fd; // Listen for incoming connections
  pfd.events = POLLIN;
  pfd.revents = 0;

  printf(
      "Waiting for network connection on port 9999 (waiting for 5 seconds):\n");
  int ret = poll(&pfd, 1, 5000); // 5-second timeout

  if (ret > 0) {
    if (pfd.revents & POLLIN) {
      print_revents(pfd.revents);
      printf("Network connection available.\n");
    }
  } else if (ret == 0) {
    printf("Timeout occurred! No connection after 5 seconds.\n");
  } else {
    perror("poll");
  }

  close(server_fd);
}

void test_unix_socket() {
  int sockets[2];
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == -1) {
    perror("socketpair");
    return;
  }

  const char *msg = "Hello, Unix Socket!";
  write(sockets[1], msg, strlen(msg)); // Write to one end of the socket pair

  struct pollfd pfd;
  pfd.fd = sockets[0]; // Monitor the other end
  pfd.events = POLLIN;
  pfd.revents = 0;

  int ret = poll(&pfd, 1, 5000); // 5-second timeout

  if (ret > 0) {
    if (pfd.revents & POLLIN) {
      print_revents(pfd.revents);
      printf("Data is available to read from the Unix socket.\n");
      char buffer[100];
      ssize_t bytesRead = read(sockets[0], buffer, sizeof(buffer) - 1);
      if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("Read from Unix socket: %s\n", buffer);
      }
    } else if (ret == 0) {
      printf("Timeout occurred! No data after 5 seconds.\n");
    } else {
      perror("poll");
    }

    close(sockets[0]);
    close(sockets[1]);
  }
}

void test_stdin() {
  struct pollfd fds;
  int ret;

  // Set up the pollfd structure
  fds.fd = 0;          // File descriptor 0 (stdin)
  fds.events = POLLIN; // Monitor for input

  // Wait for up to 5000 milliseconds
  printf("Waiting (5 seconds) for input...\n");
  ret = poll(&fds, 1, 5000);

  if (ret == -1) {
    perror("poll");
    return;
  } else if (ret == 0) {
    print_revents(fds.revents);
    printf("Timeout occurred! No input detected.\n");
  } else {
    print_revents(fds.revents);
    if (fds.revents & POLLIN) {
      printf("Data is ready to be read.\n");
      char buffer[100];
      ssize_t bytesRead = read(0, buffer, sizeof(buffer) - 1);
      if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("Read from stdin: %s\n", buffer);
      }
    }
  }
}

void test_pipe() {
  int pipefds[2];
  if (pipe(pipefds) == -1) {
    perror("pipe");
    return;
  }

  // Write some data to the write end of the pipe
  const char *msg = "Hello, POLLIN!";
  write(pipefds[1], msg, strlen(msg));

  // Set up poll to monitor the read end of the pipe for available data
  struct pollfd pfd;
  pfd.fd = pipefds[0]; // Read end of the pipe
  pfd.events = POLLIN;
  pfd.revents = 0;

  int ret = poll(&pfd, 1, 5000); // 5-second timeout

  if (ret > 0) {
    if (pfd.revents & POLLIN) {
      print_revents(pfd.revents);
      printf("Data is available to read from the pipe.\n");
      char buffer[100];
      ssize_t bytesRead = read(pipefds[0], buffer, sizeof(buffer) - 1);
      if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("Read from pipe: %s\n", buffer);
      }
    }
  } else if (ret == 0) {
    printf("Timeout occurred! No data available after 5 seconds.\n");
  } else {
    perror("poll");
  }

  close(pipefds[0]);
  close(pipefds[1]);
}

int main() {
  //   test_pipe();
  //   test_stdin();
  //   test_unix_socket();
  //   test_network_socket();
  //   test_network_socket_client_server();
  test_network_socket_client_server_message();
  return 0;
}
