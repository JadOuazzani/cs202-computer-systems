# CS-202 Computer Systems: labs in C

Labs for EPFL's CS-202 *Computer Systems* course (spring 2026), written in C by Jad Chahed Ouazzani and Riad Elhariry

The work is in [`done/`](done). The [`provided/`](provided) folder holds the course's starting code and handouts, and the [course website](https://projprogsys-epfl.github.io/) has the full specifications.

## Warm-up

- **SHA-256** hashing of files (`sha.c`)
- Small C exercises (`ex1.c`, `ex2.c`, `stats.c`)
- A chip-placement warm-up project (`bigprj/`)

## Lab 2: files and networking
[`done/Lab2/`](done/Lab2)

- **System calls**: reading, writing and deleting files with `open`, `read`, `write`, `lseek` and `close` instead of the C standard library, plus tests of concurrent access to the same file
- **UDP**: client and server exchanging probes, with the server measuring the one-way delay
- **TCP**: a multithreaded file server that receives files from clients, with one mutex per file name (kept in a hash table) so that concurrent clients never write the same file at the same time, and a client that sends a file and checks the server's acknowledgment

## Lab 4: a Unix v6 filesystem
[`done/Lab4/`](done/Lab4)

`u6fs`, a tool that reads disk images in the Unix v6 filesystem format:

- reading disk sectors, mounting and unmounting a filesystem
- reading inodes and listing every inode of a disk
- reading file blocks, printing the first sector of a file and computing the SHA-256 of a file
- opening and reading directories

## Building

Each folder has a `Makefile`: run `make` inside it. The programs target Linux (POSIX sockets and threads).
