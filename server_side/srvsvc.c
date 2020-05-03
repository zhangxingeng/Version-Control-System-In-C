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
	if(send_one_file(cli_sock, "./.manifest")<=0){return -1;}
	if(recv_one_file(cli_sock) != NULL){printf("recv commit.\n");}
	return change_mani_with_file_recv(cli_sock, "./.manifest", "./.commit");
}

/**
 * 	TODO: expire all other commit
 */
int push_srv(char* proj, int cli_sock){
	int buff_size = 1000; char buff[buff_size];
	read_cmd(cli_sock, buff, buff_size);//recv md5
	int fn_len = strlen(buff) + strlen("./.commit_")+1;//find its own commit
	char* commit_fn = calloc(fn_len, sizeof(char));
	strcpy(commit_fn, "./.commit_"); strcat(commit_fn, buff);
	//delete all other commits
	send_one_file(cli_sock, commit_fn);	//send out the manifest
	return change_mani_with_file_recv(cli_sock, "./.manifest", commit_fn);
}



int update_srv(char* proj, int cli_sock){
	send_one_file(cli_sock, "./.manifest");
	return 0;
}



int upgrade_srv(char* proj, int cli_sock){
	change_mani_with_file_send(cli_sock);
	return 0;
}



















