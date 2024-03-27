# poll

Commented and a little bit variated man page for poll

## Name

poll – synchronous I/O multiplexing

## SYNOPSIS

```c
#include <poll.h>

int poll(struct pollfd fds[], nfds_t nfds, int timeout);
```

## DESCRIPTION

poll() examines a set of file descriptors to see if some of them are ready for I/O or if certain events have occurred on them.

- The **fds** argument is a pointer to an array of pollfd structures, as defined in ⟨poll.h⟩ (shown below).
- The **nfds** argument specifies the size of the fds array.
- The **timeout** parameter specifies the duration (in milliseconds) that poll() should wait for any of the specified file descriptors to become ready for I/O operations. A timeout value of -1 indicates that poll() should wait indefinitely, a value of 0 instructs poll() to return immediately even if no file descriptors are ready (non-blocking mode), and a positive value specifies the maximum wait time.

```c
struct pollfd {
	int	fd;	  /* file descriptor */
	short	events;   /* events to look for */
	short	revents;  /* events returned */
};
```

The event bitmasks in events and revents have the following bits:

[In a bitmask, each bit represents a different flag or setting, and these bits correspond to individual values. When a bit is set to 1, it indicates that the associated setting or flag is "on" or active. Conversely, a bit set to 0 means the setting is "off" or inactive. This allows multiple flags to be stored in a single integer variable, making it an efficient way to track a combination of on/off settings.]

- **POLLERR** An exceptional condition has occurred on the device or socket. This flag is output only, and ignored if present in the input events bitmask.

- **POLLHUP** The device or socket has been disconnected. This flag is output only, and ignored if present in the input events bitmask. Note that POLLHUP and POLLOUT are mutually exclusive and should never be present in the revents bitmask at the same time.

- **POLLIN** Data other than high priority data may be read without blocking. This is equivalent to ( POLLRDNORM | POLLRDBAND).

- **POLLNVAL** The file descriptor is not open. This flag is output only, and ignored if present in the input events bitmask.

- **POLLOUT** Normal data may be written without blocking. This is equivalent to POLLWRNORM.

- **POLLPRI** High priority data may be read without blocking.

- **POLLRDBAND** Priority data may be read without blocking.

- **POLLRDNORM** Normal data may be read without blocking.

- **POLLWRBAND** Priority data may be written without blocking.

- **POLLWRNORM** Normal data may be written without blocking.

  The distinction between normal, priority, and high-priority data is
  specific to particular file types or devices.

  If timeout is greater than zero, it specifies a maximum interval (in
  milliseconds) to wait for any file descriptor to become ready. If
  timeout is zero, then poll() will return without blocking. If the value
  of timeout is -1, the poll blocks indefinitely.

## List of events as in poll.h

# poll.h Compatibility and Events

This file is intended to be compatible with the traditional `poll.h`.

## Requestable Events

These are the events that can be requested in the `events` field of a `pollfd` structure. If `poll(2)` finds any of these set in the `events` field, they are copied to `revents` on return.

- `POLLIN`: 0x0001 - Any readable data available.
- `POLLPRI`: 0x0002 - OOB/Urgent readable data.
- `POLLOUT`: 0x0004 - File descriptor is writeable.
- `POLLRDNORM`: 0x0040 - Non-OOB/URG data available.
- `POLLWRNORM`: `POLLOUT` - No write type differentiation.
- `POLLRDBAND`: 0x0080 - OOB/Urgent readable data.
- `POLLWRBAND`: 0x0100 - OOB/Urgent data can be written.

## FreeBSD Extensions

Polling on a regular file might return one of these events (currently only supported on local filesystems).

- `POLLEXTEND`: 0x0200 - File may have been extended.
- `POLLATTRIB`: 0x0400 - File attributes may have changed.
- `POLLNLINK`: 0x0800 - (Un)link/rename may have happened.
- `POLLWRITE`: 0x1000 - File's contents may have changed.

## Unconditional Events

These events are set if they occur regardless of whether they were requested.

- `POLLERR`: 0x0008 - Some poll error occurred.
- `POLLHUP`: 0x0010 - File descriptor was "hung up".
- `POLLNVAL`: 0x0020 - Requested events "invalid".

The standard events mask includes:

- `POLLSTANDARD`: `(POLLIN|POLLPRI|POLLOUT|POLLRDNORM|POLLRDBAND|POLLWRBAND|POLLERR|POLLHUP|POLLNVAL)`
