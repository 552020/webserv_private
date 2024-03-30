# select

`select()` is a system call used is socket programming to monitor multiple file descriptors, waiting until one or more of the file descriptors bexome "ready" for some I/O operation.

`int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds, struct timeval *restrict timeout);`

`select()` takes 5 parameters:

- **nfds**: The highest-numbered file descriptor in any of the three sets, plus 1.
- **readfds**: An `fd_set` pointer specifying the file descriptors to be checked for being ready to read.
- **writefds**: An `fd_set` pointer for file descriptors to be checked for being ready to write.
- **exceptfds**: An `fd_set` pointer for file descriptors to be checked for error conditions pending.
- **timeout**: A pointer to a `struct timeval` that specifies the maximum interval to wait for any file descirptor to become ready. A null pointer specifies an infinite timeout.

## select vs poll

- **Scalability**: select() has a limitation on the number of file descriptors it can monitor, typically defined by the FD_SETSIZE constant. This makes it less scalable than poll(), which does not have this limitation.

- **File Descriptor Sets vs. Arrays**: select() uses fd_set structures to manage sets of file descriptors, which can be less convenient and more cumbersome than poll()'s use of an array of pollfd structures.

- **Handling Large Numbers of File Descriptors**: Due to the FD_SETSIZE limitation and the way fd_set works, select() can be inefficient when working with a large number of file descriptors, as it requires iterating over the whole set. poll(), on the other hand, directly provides the status of each file descriptor it monitors.
