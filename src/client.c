#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "socket_lib.h"

void func(int sock_fd)
{
	char buff[1024];
	int n;
	for (;;) {
		//prep for sending
		bzero(buff, sizeof(buff));
		printf("Enter the string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n'){}
		buff[n+1] = '\0';
		send_str(buff, sock_fd);
		//recv
		bzero(buff, sizeof(buff));
		char* got = recv_str(sock_fd);

		printf("From Server : %s", got);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

int main()
{
	int sock_fd = conn_estab('c', "localhost", 10025);

	// function for chat
	func(sock_fd);


	close(sock_fd);
}




