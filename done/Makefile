CC = gcc
CFLAGS = -std=c99
LDLIBS = -lssl -lcrypto

sha: sha.c
	$(CC) $(CFLAGS) -o sha sha.c $(LDLIBS)

clean:
	rm -f sha


