#include "clisvc.h"

int create_project_cli(char* proj, int srv_sock){
	char msg[1024];
	strcpy(msg, "create ");
	strcat(msg, proj);
	write_cmd(srv_sock, msg);
	step_in_dir(proj);
	if(recv_files(srv_sock) != NULL){
		printf("Project Successfully Created.\n");
	}
	return 0;
	}
