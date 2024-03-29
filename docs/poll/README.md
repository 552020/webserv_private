# poll

The directory poll is dedicated to poking around in the poll world.

- The file `poll.md` is the starting point of the exploration. Poll.md is an annoted version of the man page of the poll man page.
- The file `poll_man_macos.txt` contains the text of the man page for poll for macos.
- The fie `poll_man_linux.txt` ... you can figure out but yourself.
- The other files are tests or notes about the possible events we are listening for with poll, or about 'revents' (returned events), i.e. the state of the fd when poll returns.
- The `print_revents` file containes a function that is used but all other test programs and they shoud be compiled together with that file: `cc pollerror_invalid.c print_revents.c`.

## TODO

- [x] POLLERR basic
