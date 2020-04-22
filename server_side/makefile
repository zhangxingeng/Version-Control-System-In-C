CFLAGS=-Wall -Werror -fsanitize=address -std=c99

all: srvsvc.o socket_lib.o msc.o files_io.o
	gcc $(CFLAGS) ./server.c srvsvc.o socket_lib.o msc.o files_io.o -o server_exec
	rm srvsvc.o socket_lib.o msc.o files_io.o

srvsvc.o:
	gcc $(CFLAGS) -c ./srvsvc.c

socket_lib.o:
	gcc $(CFLAGS) -c ./libs/socket_lib.c

msc.o:
	gcc $(CFLAGS) -c ./libs/msc.c

files_io.o:
	gcc $(CFLAGS) -c ./libs/files_io.c

clean:
	rm server_exec
