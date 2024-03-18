# Webserv

This is when you finally understand why a URL starts with HTTP.

## Summary

This project is about writing your own HTTP server. You will be able to test it with an actual browser. HTTP is one of the most used protocols on the internet. Knowing its arcane will be useful, even if you won’t be working on a website.

**Version:** 21.2

## Contents

- [Introduction](#introduction)
- [General rules](#general-rules)
- [Mandatory part](#mandatory-part)
  - [Requirements](#requirements)
  - [For MacOS only](#for-macos-only)
  - [Configuration file](#configuration-file)
- [Bonus part](#bonus-part)
- [Submission and peer-evaluation](#submission-and-peer-evaluation)

## Chapter I: Introduction

The Hypertext Transfer Protocol (HTTP) is an application protocol for distributed, collaborative, hypermedia information systems. HTTP is the foundation of data communication for the World Wide Web, where hypertext documents include hyperlinks to other resources that the user can easily access, for example, by a mouse click or by tapping the screen in a web browser. HTTP was developed to facilitate hypertext and the World Wide Web.

The primary function of a web server is to store, process, and deliver web pages to clients. The communication between client and server takes place using the Hypertext Transfer Protocol (HTTP). Pages delivered are most frequently HTML documents, which may include images, style sheets, and scripts in addition to the text content. Multiple web servers may be used for a high-traffic website.

A user agent, commonly a web browser or web crawler, initiates communication by requesting a specific resource using HTTP, and the server responds with the content of that resource or an error message if unable to do so. The resource is typically a real file on the server’s secondary storage, but this is not necessarily the case and depends on how the web server is implemented.

## Chapter II: General rules

- Your program should not crash in any circumstances (even when it runs out of memory), and should not quit unexpectedly. If it happens, your project will be considered non-functional and your grade will be 0.
- You have to turn in a Makefile which will compile your source files. It must not relink.
- Your Makefile must at least contain the rules: `$(NAME)`, `all`, `clean`, `fclean`, and `re`.
- Compile your code with `c++` and the flags `-Wall -Wextra -Werror`.
- Your code must comply with the C++ 98 standard. Then, it should still compile if you add the flag `-std=c++98`.
- Try to always develop using the most C++ features you can (for example, choose `<cstring>` over `<string.h>`). You are allowed to use C functions, but always prefer their C++ versions if possible.
- Any external library and Boost libraries are forbidden.

## Chapter III: Mandatory part

### Program name

`webserv`

### Turn in files

Makefile, `*.{h, hpp}`, `*.cpp`, `*.tpp`, `*.ipp`, configuration files

### Arguments

[A configuration file]

### External functs.

Everything in C++ 98. execve, dup, dup2, pipe, strerror, gai_strerror,
errno, dup, dup2, fork, socketpair, htons, htonl,
ntohs, ntohl, select, poll, epoll (epoll_create,
epoll_ctl, epoll_wait), kqueue (kqueue, kevent),
socket, accept, listen, send, recv, chdir bind,
connect, getaddrinfo, freeaddrinfo, setsockopt,
getsockname, getprotobyname, fcntl, close, read,
write, waitpid, kill, signal, access, stat, open,
opendir, readdir and closedir.

### Libft authorized

n/a

### Description

A HTTP server in C++ 98

---

You must write a HTTP server in C++ 98.
Your executable will be run as follows: ./webserv [configuration file]

---

Even if poll() is mentionned in the subject and the evaluation scale,
you can use any equivalent such as select(), kqueue(), or epoll().

---

Please read the RFC and do some tests with telnet and NGINX before
starting this project.
Even if you don’t have to implement all the RFC, reading it will help
you develop the required features.

---

### Requirements

- Your program has to take a configuration file as an argument, or use a default path.
- You can’t execve another web server.
- Your server must never block, and the client can be bounced properly if necessary.
- It must be non-blocking and use only 1 `poll()` (or equivalent) for all the I/O operations between the client and the server (listen included).
- `poll()` (or equivalent) must check read and write at the same time.
- You must never do a read or a write operation without going through `poll()` (or equivalent).
- Checking the value of `errno` is strictly forbidden after a read or a write operation.
- You don’t need to use `poll()` (or equivalent) before reading your configuration file.

### For MacOS only

- Since MacOS doesn’t implement `write()` the same way as other Unix OSes, you are allowed to use `fcntl()`.
- You must use file descriptors in non-blocking mode to get a behavior similar to that of other Unix OSes.
- However, you are only allowed to use `fcntl()` with the following flags: `F_SETFL`, `O_NONBLOCK`, and `FD_CLOEXEC`. Any other flag is forbidden.

### Configuration file

- You can get some inspiration from the ’server’ part of NGINX configuration file.
- In the configuration file, you should be able to choose the port and host of each ’server’, setup server_names or not, and more.

## Chapter IV: Bonus part

Here are the extra features you can add:

- Support cookies and session management.
- Handle multiple CGI.

The bonus part will only be assessed if the mandatory part is PERFECT.

## Chapter V: Submission and peer-evaluation

Turn in your assignment in your Git repository as usual. Only the work inside your repository will be evaluated during the defense.
