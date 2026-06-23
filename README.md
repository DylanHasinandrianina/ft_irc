# ft_irc

*This project has been created as part of the 42 curriculum by dagouill, shasinan*

---

# Description

ft_irc is a custom Internet Relay Chat (IRC) server developed in C++98 as part of the 42 curriculum.

The goal of the project is to recreate the core behavior of a real IRC server capable of handling multiple clients simultaneously using non-blocking I/O and `poll()`.

The server allows users to:
- authenticate with a password
- register a nickname and username
- join channels
- send and receive private messages
- manage channels with operator privileges

This project focuses on:
- TCP/IP networking
- socket programming
- multiplexing with `poll()`
- non-blocking communication
- multi-client server architecture

---

# Features

- Non-blocking sockets
- Multi-client handling using `poll()`
- TCP/IP communication
- IRC authentication system
- Channel creation and management
- Private messaging
- Operator privileges
- Channel modes:
  - `+i` : invite-only channel
  - `+t` : topic restricted to operators
  - `+k` : password-protected channel
  - `+o` : operator privileges
  - `+l` : user limit

Implemented IRC commands:
- PASS
- NICK
- USER
- JOIN
- PRIVMSG
- KICK
- INVITE
- TOPIC
- MODE

---

# Architecture

The project is divided into three main components:

## Network / Poll

Handles:
- socket creation
- binding and listening
- client connections
- non-blocking communication
- `poll()` event management

Main system calls:
- `socket()`
- `bind()`
- `listen()`
- `accept()`
- `poll()`
- `send()`
- `recv()`

---

## Parsing / Commands

Handles:
- buffering incoming data
- reconstructing partial messages
- parsing IRC commands
- command execution

Examples:
- PASS
- NICK
- USER
- JOIN
- PRIVMSG

---

## Channels / Permissions

Handles:
- channel management
- user registration inside channels
- operator privileges
- channel modes and restrictions

Examples:
- KICK
- INVITE
- TOPIC
- MODE

---

# Instructions

## Compilation

```bash
make
```

---

## Launch the server

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 pass
```

---

## Connect using an IRC client

Example using irssi:

```bash
/connect localhost 6667 pass
```

---

# Usage

Example IRC commands:

```text
PASS pass
NICK alice
USER alice 0 * :Alice
JOIN #general
PRIVMSG #general :Hello everyone
```

Example operator commands:

```text
KICK #general bob
INVITE bob #private
TOPIC #general :Welcome to the channel
MODE #general +i
```

---

# Technical Concepts

This project explores several low-level networking and system programming concepts:

- TCP/IP networking
- Socket programming
- Non-blocking file descriptors
- I/O multiplexing with `poll()`
- Multi-client server management
- Message buffering and parsing
- IRC protocol fundamentals

---

# Resources

- RFC 1459 - Internet Relay Chat Protocol
- RFC 2812 - Internet Relay Chat: Client Protocol
- Beej's Guide to Network Programming
- Linux `poll(2)` manual
- irssi documentation

Useful tools:
- irssi
- netcat (`nc`)
- valgrind
- gdb

---

# AI Usage

AI tools were used during the project for:
- understanding IRC protocol concepts
- clarifying socket and `poll()` behavior
- debugging specific networking issues
- improving documentation and README structure

All implementation decisions and final code integration were reviewed, tested, and validated by the project team.