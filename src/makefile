CFLAGS=-Wall -Werror -fsanitize=address -std=c99

all: socket_lib.o
	gcc $(CFLAGS) server.c socket_lib.o -o server
	gcc $(CFLAGS) client.c socket_lib.o -o client
	rm socket_lib.o

socket_lib.o:
	gcc $(CFLAGS) -c socket_lib.c

clean:
	rm client server 
