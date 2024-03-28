#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8888

void *server_thread_func(void *arg) {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    pthread_exit(NULL);
  }

  // Forcefully attaching socket to the port 8888
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    pthread_exit(NULL);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Forcefully attaching socket to the port 8888
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    pthread_exit(NULL);
  }
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    pthread_exit(NULL);
  }
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    pthread_exit(NULL);
  }
  read(new_socket, buffer, 1024);
  printf("Message from client: %s\n", buffer);
  close(new_socket);
  close(server_fd);
  pthread_exit(NULL);
}

void *client_thread_func(void *arg) {
  sleep(1); // Ensure server starts first
  int sock = 0;
  struct sockaddr_in serv_addr;
  char *hello = "Hello from client";

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    pthread_exit(NULL);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    pthread_exit(NULL);
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    pthread_exit(NULL);
  }
  send(sock, hello, strlen(hello), 0);
  printf("Message sent\n");
  close(sock);
  pthread_exit(NULL);
}

int main() {
  pthread_t server_thread, client_thread;

  // Create server and client threads
  pthread_create(&server_thread, NULL, server_thread_func, NULL);
  pthread_create(&client_thread, NULL, client_thread_func, NULL);

  // Wait for threads to finish
  pthread_join(server_thread, NULL);
  pthread_join(client_thread, NULL);

  return 0;
}
