
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "libs/socket_lib.h"
#include "clisvc.h"


/**
 *  convert user command into function
 */
int cmd_to_svc(int srv_sock, char* cmd, char* proj, char* arg1){
	int result = 0;

	if(strcmp(cmd, "create") == 0){
		if(project_stepin(proj, '?') != -1){return -1;}
		if(cmd_relay(proj, "create", srv_sock)!=0){return -1;}
		result = create_project_cli(proj, srv_sock);
	}else{
		if(project_stepin(proj, '?') != 0){return -1;}
		if(strcmp(cmd, "add") == 0){
			result = add_a_file_cli(proj, arg1);
		}else if (strcmp(cmd, "delete") == 0){
			result = delete_a_file_cli(proj, arg1);
		}else if(strcmp(cmd, "update") == 0){
			result = update_cli(proj, srv_sock);
		}else if(strcmp(cmd, "upgrade") == 0){
			result = upgrade_cli(proj, srv_sock);
		}else if(strcmp(cmd, "commit") == 0){
			result = commit_cli(proj, srv_sock);
		}else if(strcmp(cmd, "push") == 0){
			result = push_cli(proj, srv_sock);
		}else{
			result = -1;
		}
	}
	return result;
}



int main(int argc, char** argv){
	int sock_fd = conn_estab('C', "localhost", 12196);
	//printf("%d", cmd_to_svc(sock_fd, "push", "kuma", NULL));
	printf("%d", cmd_to_svc(sock_fd, argv[1], argv[2], NULL));
	close(sock_fd);
	return 0;
}

