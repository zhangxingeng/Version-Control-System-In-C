
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "libs/socket_lib.h"
#include "clisvc.h"



int cmd_to_svc(int cli_sock, char* cmd, char* proj){
	int result = 0;
	if(strcmp(cmd, "create") == 0){		//for create command
		result = create_project_cli(proj, cli_sock);
		if(result != 0){
			printf("got some error when creating Project.");
		}
	}else if(strcmp(cmd, "upgrade") == 0){

	}
	return result;
}

int main(int argc, char** argv){
	int sock_fd = conn_estab('c', "localhost", 10035);
	printf("%d", cmd_to_svc(sock_fd, argv[1], argv[2]));
	close(sock_fd);
}



