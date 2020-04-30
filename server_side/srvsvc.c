#include "srvsvc.h"




/**
 * 	create a project at certain path
 *
 */
int create_project_srv(char* proj, int cli_sock){
	if(project_stepin(proj, '+') != 0){return -1;}
	int fp = open("./.manifest", O_RDWR|O_CREAT, 0666);
	write(fp, "0\n", 2);//initialize version to be 0
	send_one_file(cli_sock, "./.manifest");
	return 0;
}


int commit_srv(char* proj, int cli_sock){
	send_one_file(cli_sock, "./.manifest");
	if(recv_files(cli_sock) != NULL){printf("recv commit.\n");}
	return 0;
}


int push_srv(char* proj, int cli_sock){
	//delete all other commits
	//recv files and apply to local
	//update manifiste

	return 0;
}



int update_srv(char* proj, int cli_sock){
	send_one_file(cli_sock, "./.manifest");
	return 0;
}



int upgrade_srv(char* proj, int cli_sock){
	if(recv_files(cli_sock) != NULL){printf("recv update.\n");}
	struct Project* upgrade_files = indexer_read("./.update");
	send_files(upgrade_files, cli_sock);
	return 0;
}




















