CFLAGS=#-Wall -Werror -fsanitize=address -std=c99
I=-I /usr/local/Cellar/openssl@1.1/1.1.1g/include
L=-L /usr/local/Cellar/openssl@1.1/1.1.1g/lib


all: srvsvc.o socket_lib.o msc.o files_io.o ll.o
	gcc $(CFLAGS) ./server.c \
	srvsvc.o socket_lib.o msc.o files_io.o ll.o \
	-o ./working/server_exec $(I) $(L) -lcrypto -lssl
	
	rm srvsvc.o socket_lib.o msc.o files_io.o ll.o

srvsvc.o:
	gcc $(CFLAGS) -c ./srvsvc.c $(I)

socket_lib.o:
	gcc $(CFLAGS) -c ./libs/socket_lib.c

msc.o:
	gcc $(CFLAGS) -c ./libs/msc.c $(I)

files_io.o:
	gcc $(CFLAGS) -c ./libs/files_io.c $(I)

ll.o:
	gcc $(CFLAGS) -c ./libs/ll.c

clean:
	rm ./working/server_exec
