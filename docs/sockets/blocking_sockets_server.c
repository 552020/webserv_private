#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define TRUE 1

/*
The purpose of this program is to demonstrate the use of blocking sockets.
We have a server that listens for incoming connections and echos back the data
that the client sends. The server can handle multiple cients, i.e. accepting
multiple connections, i.e. being connected to multiple clients at the same time
but it can only handle one client at a time. This is because we are using
blocking sockets. This means the server can be connected with multiple clients
at the same time but the clients will need to wait the other client to finish
before they can send data to the server. This is because the server is using
blocking sockets. The server will block on the read call until it receives data
from the client. This means that the server can only handle one client at a
time. If the server is connected to a client and another client tries to
connect, the server will not be able to accept the connection until the first
client disconnects. This is because the server is using blocking sockets. If you
want to handle multiple clients at the same time, you need to use non-blocking
sockets.


*/

int main() {
  int server_fd, new_socket, client_socket[MAX_CLIENTS],
      max_clients = MAX_CLIENTS, activity, i, valread, sd, addrlen;
  int max_sd;
  struct sockaddr_in address;
  char buffer[1025]; // data buffer of 1K

  // set of socket descriptors
  fd_set readfds;
  char *message =
      "Enter '1', then '2', then '3', then 'done' to close connection\n";

  // initialise all client_socket[] to 0 so not checked
  for (i = 0; i < max_clients; i++) {
    client_socket[i] = 0;
  }

  // create a master socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // bind the socket to localhost port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // try to specify maximum of 3 pending connections for the master socket
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // accept the incoming connection
  addrlen = sizeof(address);
  puts("Waiting for connections ...");

  while (TRUE) {
    // clear the socket set
    FD_ZERO(&readfds);

    // add master socket to set
    FD_SET(server_fd, &readfds);
    max_sd = server_fd;

    // add child sockets to set
    for (i = 0; i < max_clients; i++) {
      // socket descriptor
      sd = client_socket[i];

      // if valid socket descriptor then add to read list
      if (sd > 0)
        FD_SET(sd, &readfds);

      // highest file descriptor number, need it for the select function
      if (sd > max_sd)
        max_sd = sd;
    }

    // wait for an activity on one of the sockets, timeout is NULL, so wait
    // indefinitely
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno != EINTR)) {
      printf("select error");
    }

    // If something happened on the master socket, then it's an incoming
    // connection
    if (FD_ISSET(server_fd, &readfds)) {
      if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                               (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      // inform user of socket number - used in send and receive commands
      printf("New connection, socket fd is %d, ip is : %s, port : %d\n",
             new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

      // send new connection greeting message
      if (send(new_socket, message, strlen(message), 0) != strlen(message)) {
        perror("send");
      }

      puts("Welcome message sent successfully");

      // add new socket to array of sockets
      for (i = 0; i < max_clients; i++) {
        if (client_socket[i] == 0) {
          client_socket[i] = new_socket;
          printf("Adding to list of sockets as %d\n", i);
          break;
        }
      }
    }

    // else it's some IO operation on some other socket
    for (i = 0; i < max_clients; i++) {
      sd = client_socket[i];

      if (FD_ISSET(sd, &readfds)) {
        // Check if it was for closing, and also read the incoming message
        if ((valread = read(sd, buffer, 1024)) == 0) {
          // Somebody disconnected, get his details and print
          getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
          printf("Host disconnected, ip %s, port %d\n",
                 inet_ntoa(address.sin_addr), ntohs(address.sin_port));

          // Close the socket and mark as 0 in list for reuse
          close(sd);
          client_socket[i] = 0;
        } else {
          // Echo back the message that came in
          buffer[valread] = '\0';
          send(sd, buffer, strlen(buffer), 0);
        }
      }
    }
  }

  return 0;
}
