#include "socket_lib.h"

/**
 *	establish connection for client/server
 * 	connect to or bind with an ip and port
 * 	return the socket for sending messages
 */
int conn_estab(char type, char* name, int port){
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {printf("Error 1.\n");exit(0);}

	struct hostent* results = gethostbyname("localhost");
	struct sockaddr_in serv_addr;
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(10027);//host to network short
	bcopy(results->h_addr, &(serv_addr.sin_addr.s_addr), results->h_length);
	if(type == 'c'){
		if(connect(sock_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) != 0){
			printf("Error 2.\n");exit(0);}else{printf("connected.\n");}
		return sock_fd;
	}else if(type == 'b'){
		if ((bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) != 0)
		{printf("Error 3.\n");exit(0);}else{printf("binded.\n");}
		return sock_fd;
	}else{return -1;}
}



