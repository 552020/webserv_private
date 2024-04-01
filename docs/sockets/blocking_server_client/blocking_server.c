#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080

void handle_connection(int sock) {
  char buffer[1024];
  ssize_t n;

  // Echo service: Read from socket and write back what was read
  while ((n = read(sock, buffer, sizeof(buffer) - 1)) > 0) {
    buffer[n] = '\0';
    printf("Server received: %s\n", buffer);
    write(sock, buffer, n);
  }

  if (n < 0) {
    perror("Error reading from socket");
  }

  printf("Connection closed by client\n");
  close(sock);
}

int main() {
  // server_fd is the file descriptor for the server socket, the listening
  // socket new_socket is the file descriptor for the client socket, the socket
  // returned by accept()
  int server_fd, new_socket;
  // address is used for a double purpose: to bind the socket to the address and
  // port and to store the address of the client that connected to the server
  // we could also have used two different structs for this like
  // server_address and client_address
  struct sockaddr_in address;
  // The flag is used to set the socket options and means jusn ON (1) or OFF (0)
  int opt = 1;
  int addrlen = sizeof(address);

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Binding the socket to the address and port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Start listening for connections
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on port %d...\n", PORT);

  while (1) {
    printf("Waiting for a connection...\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    printf("Connection established\n");

    // Handle connection (blocking operation)
    handle_connection(new_socket);

    // Close connection and wait for the next one
    printf("Ready for new connection\n");
  }

  return 0;
}
