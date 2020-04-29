CFLAGS=-Wall -Werror -fsanitize=address -std=c99
I=-I /usr/local/Cellar/openssl@1.1/1.1.1g/include
L=-L /usr/local/Cellar/openssl@1.1/1.1.1g/lib


all: clisvc.o socket_lib.o msc.o files_io.o ll.o
	gcc $(CFLAGS) ./client.c \
	clisvc.o socket_lib.o msc.o files_io.o ll.o \
	-o ./working/client_exec $(I) $(L) -lcrypto -lssl
	
	rm clisvc.o socket_lib.o msc.o files_io.o ll.o

clisvc.o:
	gcc $(CFLAGS) -c ./clisvc.c $(I)

socket_lib.o:
	gcc $(CFLAGS) -c ./libs/socket_lib.c

msc.o:
	gcc $(CFLAGS) -c ./libs/msc.c $(I)

files_io.o:
	gcc $(CFLAGS) -c ./libs/files_io.c $(I)

ll.o:
	gcc $(CFLAGS) -c ./libs/ll.c

clean:
	rm ./working/client_exec
