CFLAGS=-Wall -Werror -fsanitize=address -std=c99

all: clisvc.o socket_lib.o msc.o files_io.o
	gcc $(CFLAGS) ./client.c clisvc.o socket_lib.o msc.o files_io.o -o client_exec
	rm clisvc.o socket_lib.o msc.o files_io.o

clisvc.o:
	gcc $(CFLAGS) -c ./clisvc.c

socket_lib.o:
	gcc $(CFLAGS) -c ./libs/socket_lib.c

msc.o:
	gcc $(CFLAGS) -c ./libs/msc.c

files_io.o:
	gcc $(CFLAGS) -c ./libs/files_io.c

clean:
	rm client_exec
