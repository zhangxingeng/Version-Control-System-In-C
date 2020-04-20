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


int send_str(char* msg, int fd){
	int cnt = 0, len = strlen(msg);
	while(cnt != len){
		cnt += write(fd, msg+cnt, len-cnt);

	}
	return cnt;
}
/**
 * 	recv string from file descriptor
 * 	cut off when eof or \n detected
 */
int recv_str(int fd, char* buff, int buff_size){
	memset(buff, '\0', buff_size * sizeof(char));
	char* ptr = buff, *str = NULL;
	int curr_cnt = 0, read_cnt = 0;

	do{
		if(ptr >= buff+buff_size){printf("Error.\n");return -1;}
		curr_cnt = read(fd, ptr, 1);
		if(curr_cnt < 0){printf("failed to read file\n"); return -1;}
		// tokenizes string from stream
		if (*ptr == '\n'||curr_cnt == 0) {
			if(ptr+1 >= buff+buff_size){printf("Error.\n");return -1;}
			*(ptr+1) = '\0';
			str = (char*) malloc((strlen(buff)+1) * sizeof(char));
			strcpy(str, buff);
			break;
		}
		read_cnt += curr_cnt;
		ptr += curr_cnt;
	}while(curr_cnt > 0);
	return 0;

}
