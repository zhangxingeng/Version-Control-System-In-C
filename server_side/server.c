#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#include "libs/socket_lib.h"
#include "libs/msc.h"
#include "srvsvc.h"


// Function designed for chat between client and server.
void* request_handler(void* sockfd_ptr){
	int sock_fd = *((int*)sockfd_ptr);
	int buff_size = 1024;
	char buff[buff_size];
	read_cmd(sock_fd, buff, buff_size);
	char* cmd = strtok(buff, " ");
	char* proj_name = strtok(NULL, "\0");
	cmd_to_svc(sock_fd, cmd, proj_name);
	return NULL;
}


/**
 *
 * things to do when ctrl+c is detected
 *
 */
void interrupt_Handler(){

}
// Driver function



int main(){
	//signal(SIGINT, interrupt_Handler);
	int sock_fd = conn_estab('b', "localhost", 10035);

	if ((listen(sock_fd, 5)) != 0) {printf("Listen failed...\n");exit(0);}else{printf("listening.\n");}
	while(1){
		struct sockaddr_in cli;
		int len = sizeof(cli);

		int cli_sock = accept(sock_fd, (struct sockaddr*)&cli, (unsigned int*)&len);
		if (cli_sock < 0) {printf("Error 5.\n");exit(0);}
		else{printf("Cli Accepted.\n");}
		  pthread_t cli_thread;
		  pthread_mutex_t cli_mutex;
		  pthread_mutex_init(&cli_mutex, NULL);
		  if(pthread_create(&cli_thread, NULL,request_handler, &cli_sock) != 0){printf("Error.\n");}
	}

	close(sock_fd);
}
