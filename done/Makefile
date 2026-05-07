CC = gcc
CFLAGS = -std=c99
LDLIBS = -lssl -lcrypto

sha: sha.c
	$(CC) $(CFLAGS) -o sha sha.c $(LDLIBS)

stats: stats.c
	gcc -std=c99 -pedantic -g -Wall -Wextra -o stats stats.c -lm

clean:
	rm -f sha


