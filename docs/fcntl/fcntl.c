#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  int fd = open("testfile.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  const char *data = "Hello, world!";
  size_t data_size = strlen(data);
  ssize_t bytes_written;

  while (data_size > 0) {
    bytes_written = write(fd, data, data_size);
    if (bytes_written == -1) {
      perror("write");
      return 1;
    }
    data_size -= bytes_written;
    data += bytes_written;
    printf("Bytes written: %zd\n", bytes_written);
  }

  close(fd);
  return 0;
}
