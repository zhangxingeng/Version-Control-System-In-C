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




/**
 *
 * 	fetch manifest and check for local
 *
 */
int commit_srv(char* proj, int cli_sock){
	//send manifest upon request
	//recv commit

	return 0;
}

/**
 *
 * 	fetch manifest and check for local
 *
 */
int push_srv(char* proj, int cli_sock){
	//delete all other commits
	//recv files and apply to local
	//update manifiste

	return 0;
}

/**
 *
 * 	send latest manifest upon request
 *
 */
int update_srv(char* proj, int cli_sock){
	send_one_file(cli_sock, "./.manifest");
	return 0;
}

/**
 *
 * 	fetch manifest and check for local
 *
 */
int upgrade_srv(char* proj, int cli_sock){
	if(recv_files(cli_sock) != NULL){printf("recv update.\n");}
	struct Project* upgrade_files = indexer_read("./.update");
	send_files(upgrade_files, cli_sock);
	return 0;
}




















