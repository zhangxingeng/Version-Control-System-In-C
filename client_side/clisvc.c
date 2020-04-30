#include "clisvc.h"



/**
 *
 * create a folder ready for project
 */
int create_project_cli(char* proj, int srv_sock){
	if(project_stepin(proj, '+') != 0){return -1;}
	if(cmd_relay(proj, "create", srv_sock)!=0){return -1;}

	if(recv_files(srv_sock) != NULL){printf("proj Created.\n");}
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



/**
 *
 * 	fetch manifest and check for local
 *
 */
int commit_cli(char* proj, int srv_sock){
	//fetch commit
	//find diffs mad->commit show conflict
	//send out commit to server

	return 0;
}

/**
 *
 * 	fetch manifest and check for local
 *
 */
int push_cli(char* proj, int srv_sock){
	//send out commit hash
	//send out files

	return 0;
}

/**
 *
 * 	fetch manifest and check for local
 *
 * 	need test
 *
 */
int update_cli(char* proj, int srv_sock){
	if(cmd_relay(proj, "create", srv_sock)!=0){return -1;}
	if(recv_files(srv_sock) != NULL){printf("recv mani.\n");}
	struct Project* mani_local = indexer_read("./.manifest");
	struct Project* mani_remote = indexer_read("./.manifest_remote");
	int update_fd = open("./.update", O_WRONLY|O_CREAT, 0666);
	int conflict_fd = open("./.conflict", O_WRONLY|O_CREAT, 0666);
	write(update_fd,"update\n", 7); write(conflict_fd,"conflict\n", 9);
	find_diff(mani_local, mani_remote,update_fd, conflict_fd);
	close(update_fd); close(conflict_fd);
	free_proj(mani_local);
	free_proj(mani_remote);
	if(file_len("./.conflict")<=9){remove("./.conflict");printf("ready to upgrade.\n");}
	else{printf("Please resolve the conflict first.\n");}//TODO: print out conflict info
	if(file_len("./.update")<=7){remove("./.update");printf("Your proj is up to date.\n");}
	return 0;
}

/**
 *
 * 	fetch manifest and check for local
 *	TODO: update manifest
 */
int upgrade_cli(char* proj, int srv_sock){
	if(dir_exist("./.update")!=0 || dir_exist("./.conflict")==0)
	{printf("Please update/resolve conflict first.\n");}
	if(send_one_file(srv_sock, "./.update")==0){printf("update sent.\n");}
	struct Project* upgrade_files = recv_files(srv_sock);
	//update local .manifest

	return 0;
}


int write_file_node(int fd, struct Node* file, char action){
	if(action == ' '){

	}else if(action == 'M' || action == 'A' || action == 'D' || action == 'C'){

	}
	return 0;
}



/**
 * 	find the change from remote to local,
 * 	save to update file and conflict file
 *
 * 	need test
 * 	TODO: write to files
 */
int find_diff(struct Project* local, struct Project* remote,
		int update_file, int conflict_file){
	merge_sort(local->files, comp_file_name);
	merge_sort(remote->files, comp_file_name);
	struct Node* curr_loco = local->files, *curr_remo = remote->files;
	while(curr_loco != NULL){
		while(curr_remo != NULL){
			int cmp = comp_file_name(curr_loco->data, curr_remo->data);
			if(cmp < 0){
				//write to update: D
				curr_loco = curr_loco->next;
			}else if(cmp > 0){
				//write to update A
				curr_remo = curr_remo->next;
			}else{
				//same keep checking
				//check version&&hash->
				//		same->check local file hash diff-> M
				//									same->keep going
				//		diff->Conflict C
				curr_loco = curr_loco->next;
				curr_remo = curr_remo->next;
			}
		}
	}
	return 0;
}















