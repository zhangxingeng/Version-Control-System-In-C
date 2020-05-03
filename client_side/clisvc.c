#include "clisvc.h"



/**
 *
 * create a folder ready for project
 */
int create_project_cli(char* proj, int srv_sock){
	if(project_stepin(proj, '+') != 0){return -1;}
	if(cmd_relay(proj, "create", srv_sock)!=0){return -1;}

	if(recv_proj(srv_sock) != -1){printf("proj Created.\n");}
	return 0;
}

/*
 *	local function
 *	add a file to local manifest
 * 	TODO: make sure no repetitive file is added
 */
int add_a_file_cli(char* proj_name, char* file_name){
	char* file_path = add_dot_slash(file_name);
	if(dir_exist(file_path) == -1){return -1;}
	char* md5 = gen_md5(file_name);

	//append ver, path, hash\n
	if(dir_exist("./.manifest") == -1){return -1;}
	int fd = open("./.manifest", O_WRONLY|O_APPEND);
	write_str(fd, "0", 1);write_str(fd, " ", 1);
	write_str(fd, file_path, strlen(file_path));write_str(fd, " ", 1);
	write_str(fd, md5, strlen(md5));write_str(fd, "\n", 1);
	free(md5);
	free(file_path);
	printf("file %s added.\n", file_name);
	return 0;
}


/**
 *	local function
 *  remove a record of a file from local manifest
 */
int delete_a_file_cli(char* proj_name, char* file_name){
	//read in manifest, search for file, remove from ll, mani to file
	struct Project* proj = indexer_read("./.manifest");
	char* file_path = add_dot_slash(file_name);
	struct File file; file.name = file_path;
	union Data a = {.ptr = (char*)&file};
	struct Node* result = search(proj->files, a, comp_file_name);
	free(file_path);
	delete_file_node(proj, result);
	manifest_write(proj);
	printf("file %s deleted.\n", file_name);
	return 0;
}



int commit_cli(char* proj, int srv_sock){
	if(cmd_relay(proj, "commit", srv_sock)!=0){return -1;}
	if(record_change_repo(srv_sock, proj, "commit")!=0){return -1;}
	if(send_one_file(srv_sock, "./.commit")<= 0){return -1;}
	return 0;
}


int push_cli(char* proj, int srv_sock){
	if(dir_exist("./.commit") != 0){printf("please commit before push.\n"); return -1;}
	if(dir_exist("./.conflict") == 0){printf("please resolve conflict first.\n"); return -1;}
	if(cmd_relay(proj, "push", srv_sock)!=0){return -1;}
	char buff[100];
	char* md5 = gen_md5("./.commit");
	strcpy(buff, md5);
	write_cmd(srv_sock, buff);//send out md5 of commit
	change_mani_with_file_send(srv_sock);//recv commit from srver and send out its required file
	return 0;
}


int update_cli(char* proj, int srv_sock){
	if(cmd_relay(proj, "update", srv_sock)!=0){return -1;}
	return record_change_repo(srv_sock, proj, "update");
}


int upgrade_cli(char* proj, int srv_sock){
	if(dir_exist("./.update")!=0 || dir_exist("./.conflict")==0)
	{printf("Please update/resolve conflict first.\n");}
	return change_mani_with_file_recv(srv_sock, "./.manifest", "./.update");
}














