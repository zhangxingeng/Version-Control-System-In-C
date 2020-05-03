#include "files_io.h"

/*****Functions For file List******/


/*print one node*/
void print_node(struct Node* node){
	struct File* file_info = (struct File*)(node->data.ptr);
	printf("%s->", file_info->name);  //change this for the thing you want to print
}
/*print a list out to cmd*/
void print_list(struct Node* head){
	traverse(head, print_node);
}


/**
 * 	send files to recv socket
 */
int send_files(struct Project* proj, int sock){
	long int total_byte = 10; //len of "sendfiles "
	total_byte += (int_len(proj->proj_version)+1); //<ver#>:
	total_byte += (int_len(proj->cnt)+1); //<file_#>:
	struct Node* ptr = proj->files;
	struct File* curr_file = NULL;

	while(ptr != NULL){
		curr_file = (struct File*)(ptr->data.ptr);
		int file_name_len= strlen(curr_file->name);
		int file_hash_len = strlen(curr_file->hash);
		total_byte += (file_name_len+1);//<file_name_len>:
		total_byte += (int_len(curr_file->len)+1); //<file_len_len>:
		total_byte += (file_hash_len+1);//<file_hash_len>:
		total_byte += file_len(curr_file->name);//file_len
		ptr = ptr->next;
	}
	total_byte++;//for \0
	printf("memory malloced: %ld\n",total_byte);

	char* stream = (char*)calloc(total_byte, sizeof(char));
	strcpy(stream, "sendfiles ");
	char buff[40];
	sprintf(buff, "%d", proj->proj_version);
	strcat(stream, buff);strcat(stream, ":");
	sprintf(buff, "%d", proj->cnt);
	strcat(stream, buff);strcat(stream, ":");

	ptr = proj->files;
	while(ptr != NULL){
		curr_file = (struct File*)(ptr->data.ptr);
		strcat(stream, curr_file->name); strcat(stream, ":");
		sprintf(buff, "%d", curr_file->len);
		strcat(stream, buff); strcat(stream, ":");
		strcat(stream, curr_file->hash); strcat(stream, ":");
		ptr = ptr->next;
	}

	ptr = proj->files;
	while(ptr != NULL){
		curr_file = (struct File*)(ptr->data.ptr);
		char* file_content = read_file_to_mem(curr_file->name);
		strcat(stream, file_content);
		free(file_content);
		ptr = ptr->next;
	}
	int total_sent = write_str(sock, stream, strlen(stream));
	free(stream);
	return total_sent;
}

/**
 *	return 1 if success else failed
 */
int send_one_file(int sock, char* file_name){
	struct Project project;
	project.cnt = 1;
	struct Node node; struct File file;
	project.files = &node; node.data.ptr = &file;node.next = NULL;
	struct File* curr = (struct File*)(node.data.ptr);
	project.cnt = 1;
	curr->name = file_name;
	curr->len = file_len(file_name);
	curr->hash = gen_md5(file_name);
	return send_files(&project, sock);
}

/**
 * 	compare filename inside union Data
 */
int comp_file_name(union Data a, union Data b){
	return strcmp(((struct File*)(a.ptr))->name, ((struct File*)(b.ptr))->name);
}



/**
 *
 *	delete a node from project
 */
int delete_file_node(struct Project* proj, struct Node* node){
	proj->files = delete_node(proj->files, node, free_file);
	proj->cnt--;
	return 0;
}


/**
 * recv files from a socket
 *
 * TODO: handle repetitive files
 * need to free
 */
struct Project* recv_files(int sock){
	struct Project* proj = calloc(1, sizeof(struct Project));
	int buff_size = 1000;
	char buff[buff_size];
	if(read_token(sock, buff, buff_size, ' ') <=0){free_proj(proj); return NULL;}
	read_token(sock, buff, buff_size, ':');	//proj_ver
	proj->proj_version = atoi(buff);
	read_token(sock, buff, buff_size, ':');	//file number
	proj->cnt = atoi(buff);
	struct Node* prev = NULL, *curr = NULL;
	struct File* curr_file = NULL;

	for(int i = 0; i < proj->cnt; i++){
		curr = calloc(1, sizeof(struct Node));
		curr_file= curr->data.ptr = calloc(1, sizeof(struct File));
		read_token(sock, buff, buff_size, ':');	//file name
		curr_file->name = buff_to_str(buff, buff_size);
		read_token(sock, buff, buff_size, ':');	//file length
		curr_file->len = atoi(buff);
		read_token(sock, buff, buff_size, ':');	//file hash
		curr_file->hash = buff_to_str(buff, buff_size);

		if(prev == NULL){proj->files = curr;}
		else{prev->next = curr;}
		prev = curr;
	}

	curr = proj->files;
	char* file_buff = NULL;
	while(curr != NULL){		//unpack files
		curr_file = (struct File*)(curr->data.ptr);
		file_buff = parse_file_from_readin(sock, curr_file->len);
		if(strcmp(curr_file->name, "./.commit") == 0){
			int name_len = strlen(curr_file->name) + strlen(curr_file->hash) + 1;
			char* new_name = calloc(name_len+1, sizeof(char));
			strcpy(new_name, curr_file->name);strcat(new_name, "_");
			strcat(new_name, curr_file->hash);
			free(curr_file->name);
			curr_file->name = new_name;
		}
		if(dir_exist(curr_file->name)==0){		//when file already exists
			if(strcmp(curr_file->name, "./.manifest") == 0){
				char* new_name = "./.manifest_remote";
				free(curr_file->name);
				curr_file->name = calloc(strlen(new_name)+1, sizeof(char));
				strcpy(curr_file->name, new_name);
			}else{
				remove(curr_file->name);
			}
		}

		int fd = open(curr_file->name, O_WRONLY|O_CREAT, 0666);
		write_str(fd, file_buff, curr_file->len);
		free(file_buff);
		close(fd);
		curr = curr->next;
	}
	return proj;
}

/**
 *  recv multiple files return its version
 */
int recv_proj(int sock){
	struct Project* proj = recv_files(sock);
	int ver = proj->proj_version;
	free_proj(proj);
	return ver;
}


/**
 *
 * recv one file return its name
 * need to free
 */
char* recv_one_file(int sock){
	struct Project* proj = recv_files(sock);
	if(proj->cnt != 1){printf("cannot use recv more than one file here.\n");return NULL;}
	else{
		char* fn = ((struct File*)(proj->files->data.ptr))->name;
		char* ret = calloc(strlen(fn)+1, sizeof(char));
		strcpy(ret, fn);
		free_proj(proj);
		return ret;
	}
}

void free_file(union Data file){
	struct File* file_data = (struct File*)(file.ptr);
	if(file_data->hash != NULL){free(file_data->hash);}
	if(file_data->name != NULL){free(file_data->name);}
	free(file_data);
}


void free_proj(struct Project* proj){
	struct Node* ptr = proj->files;
	while(ptr != NULL){
		free_node(ptr, free_file);
		ptr = ptr->next;
	}
	if(proj->proj_name != NULL){free(proj->proj_name);}
	free(proj);
}


/**
 * 	convert manifest to project
 * 	project must be in current dir
 *
 * 	need test
 */
struct Project* indexer_read(char* name){
	if(dir_exist(name) == -1){return NULL;}
	struct Project* proj = calloc(1, sizeof(struct Project));
	int fd = open(name, O_RDONLY);

	int buff_size = 1000;
	char buff[buff_size];
	read_token(fd, buff, buff_size, '\n');
	if(str_is_number(buff)){
		proj->proj_version = atoi(buff);
	}//either version or update/commit/conflict
	else{proj->proj_version = -1;}//version -1 means update or commit
	struct Node* prev = NULL, *curr_node = NULL;
	struct File* curr_file = NULL;
	while(1){
		if(read_token(fd, buff, buff_size, ' ') <= 0){break;}//either madc or version
		curr_node = calloc(1, sizeof(struct Node));
		curr_file = calloc(1,sizeof(struct File));
		curr_node->data.ptr = curr_file;//link file to node

		if(proj->proj_version == -1){			//for madc
			curr_file->act = buff[0];
			read_token(fd, buff, buff_size, ' ');//file version
		}
		curr_file->ver = atoi(buff);
		read_token(fd, buff, buff_size, ' ');curr_file->name = buff_to_str(buff, buff_size);
		read_token(fd, buff, buff_size, '\n');curr_file->hash = buff_to_str(buff, buff_size);
		curr_file->len = file_len(curr_file->name);
		proj->cnt++;
		if(proj->files == NULL){proj->files = curr_node;}//link node to list
		else{prev->next = curr_node;}
		prev = curr_node;
	}
	close(fd);
	return proj;
}

/**
 *
 */
int manifest_write(struct Project* proj){
	if(proj == NULL){return -1;}
	remove("./.manifest");
	int fd = open("./.manifest", O_WRONLY|O_CREAT, 0666);
	char buff[1000];
	sprintf(buff, "%d\n", (proj->proj_version+1)); write_str(fd, buff, strlen(buff));
	struct Node* n_ptr = proj->files; struct File* f_ptr = NULL;
	while(n_ptr != NULL){
		f_ptr = (struct File*)(n_ptr->data.ptr);
		sprintf(buff, "%d ", f_ptr->ver); write_str(fd, buff, strlen(buff));
		write_str(fd, f_ptr->name, strlen(f_ptr->name)); write_str(fd," ",strlen(" "));
		write_str(fd, f_ptr->hash, strlen(f_ptr->hash)); write_str(fd,"\n",strlen("\n"));
		n_ptr = n_ptr->next;
	}
	close(fd);
	return 0;
}


/*
 *	write the file change to a file
 */
int record_file_change(int fd, struct Node* fn){
	struct File* file = (struct File*)(fn->data.ptr);
	char buff[20];

	if(file->act != '\0'){
		sprintf(buff, "%c", file->act);
		write_str(fd, buff, strlen(buff));write_str(fd, " ", 1);
	}
	sprintf(buff, "%d", file->ver);
	write_str(fd, buff, strlen(buff));write_str(fd, " ", 1);
	write_str(fd, file->name, strlen(file->name));write_str(fd, " ", 1);
	write_str(fd, file->hash, strlen(file->hash));write_str(fd, "\n", 1);
	return 0;
}


/**
 * 	find the change from remote to local,
 * 	save to update file and conflict file
 *	cmd is c or u
 * 	need test
 *
 *
 */
int find_diff_repo(struct Project* loco, struct Project* remo,
		int mad_fd, int c_fd, char cmd){
	loco->files = merge_sort(loco->files, comp_file_name);
	remo->files = merge_sort(remo->files, comp_file_name);
	struct Node* node_loco = loco->files, *node_remo = remo->files;
	struct Node* prev_loco = NULL, *prev_remo = NULL;
	struct File* f_loco = NULL, *f_remo = NULL;
	int proj_ver_diff = !(loco->proj_version == remo->proj_version);
	if((cmd == 'U') && (!proj_ver_diff)){return 0;}//update no change
	if((cmd == 'C') && (proj_ver_diff)){printf("please update your local mani first.\n");return -1;}
	//here ver are diff
	while(1){
		if(node_loco == NULL && node_remo == NULL){break;}
		if(node_loco != NULL){f_loco = (struct File*)(node_loco->data.ptr);}
		else{f_loco = NULL;}
		if(node_remo != NULL){f_remo = (struct File*)(node_remo->data.ptr);}
		else{f_remo = NULL;}
		int cmp = 0;
		if(f_loco == NULL){cmp = 1;}if(f_remo == NULL){cmp = -1;}
		if(f_loco != NULL && f_remo != NULL){
			cmp = comp_file_name(node_loco->data, node_remo->data);
		}
		if(cmp < 0){	//od have, ud dont
			if(cmd == 'U'){f_loco->act = 'D';}else{f_loco->act = 'A';}
			record_file_change(mad_fd, node_loco);
			node_loco = node_loco->next;
		}else if(cmp > 0){	//ud have od dont
			if(cmd == 'U'){f_remo->act = 'A';}else{f_remo->act = 'D';};
			record_file_change(mad_fd, node_remo);
			node_remo = node_remo->next;
		}else{
			int f_live_remo_hash_diff = strcmp(f_loco->hash, f_remo->hash);
			int f_live_loco_hash_diff = strcmp(gen_md5(f_loco->name), f_loco->hash);
			if(cmd == 'U'){
				if(!f_live_loco_hash_diff){	//no local change
					if(f_live_remo_hash_diff){	//remote has changed
						f_remo->act = 'M';
						record_file_change(mad_fd, node_remo);
					}else{
						f_remo->act = 'S';
						record_file_change(mad_fd, node_remo);
					}
				}else{
					f_remo->act = 'C';
					record_file_change(c_fd, node_remo);
				}
			}else{
				if(!f_live_remo_hash_diff){
					if(f_live_loco_hash_diff){
						f_loco->act = 'M';
						record_file_change(mad_fd, node_loco);
					}else{
						f_loco->act = 'S';
						record_file_change(mad_fd, node_loco);
					}
				}else{
					f_remo->act = 'C';
					printf("You also have a project version error.\n");
					return -1;
				}
				//Node same live hash diff
				//then M
				//else C or S
			}
			if(node_loco != NULL){prev_loco = node_loco;node_loco = node_loco->next;}
			if(node_remo != NULL){prev_remo = node_remo;node_remo = node_remo->next;}
		}
	}
	return 0;
}


/**
 * cmd has to be either c(ommit)or u(pdate)
 */
int record_change_repo(int srv_sock, char* proj, char* cmd){
	if(strcmp(cmd,"commit")!=0 && strcmp(cmd,"update")!=0){return -1;}
	if(recv_one_file(srv_sock) != NULL){printf("recv mani.\n");}
	struct Project* mani_local = indexer_read("./.manifest");
	struct Project* mani_remote = indexer_read("./.manifest_remote");
	if(mani_local == NULL || mani_remote == NULL){return -1;}

	char mad_file[10] = "./."; strcat(mad_file, cmd);
	int mad_fd = open(mad_file, O_WRONLY|O_CREAT, 0666);
	int conflict_fd = open("./.conflict", O_WRONLY|O_CREAT, 0666);
	write(mad_fd, cmd, 6); write(mad_fd,"\n", 1);
	write(conflict_fd,"conflict\n", 9);
	find_diff_repo(mani_local, mani_remote,mad_fd, conflict_fd, toupper(cmd[0]));
	close(mad_fd); close(conflict_fd);
	free_proj(mani_local);
	free_proj(mani_remote);
	if(file_len("./.conflict")<=9){remove("./.conflict");printf("ready to upgrade.\n");}
	else{printf("Please resolve the conflict first.\n");return -1;}//TODO: print out conflict info
	if(file_len(mad_file)<=7){remove(mad_file);printf("Your proj is up to date.\n");}
	remove("./.manifest_remote");
	return 0;
}

int manifest_update(struct Project* mods, struct Project* mani){
	mods->files = merge_sort(mods->files, comp_file_name);
	mani->files = merge_sort(mani->files, comp_file_name);
	struct Node* node_mod = mods->files, *node_mani = mani->files, *mani_last = mani->files;
	struct File* f_mod = NULL, * f_mani = NULL;
	int cmp = 0;
	while(1){

		if(node_mod == NULL){break;}//change list is empty, get out
		else if(node_mani == NULL){cmp = 1;f_mod = (struct File*)(node_mod->data.ptr);}
		else{
			f_mod = (struct File*)(node_mod->data.ptr);
			f_mani = (struct File*)(node_mani->data.ptr);
			cmp = comp_file_name(node_mani->data, node_mod->data);
			while(mani_last->next != NULL){mani_last = mani_last->next;}
		}
		if(cmp < 0){node_mani = node_mani->next;}//no change to curr local file
		else if(cmp > 0){
			if(f_mod->act == 'A'){//append node to tail
				struct Node* new_node_mod = node_mod->next;
				mods->files = node_extract(node_mod, mods->files);
				if(node_mani != NULL){mani->files = add_before(node_mani, node_mod, mani->files);}
				else{mani->files = append_at_tail(mani_last, mani->files, node_mod);mani_last = node_mod;}
				f_mod->ver++;mods->cnt--; mani->cnt++;
				node_mod = new_node_mod;
			}
		}else{
			if(f_mod->act == 'D'){//remove node
				struct Node* temp = node_mani->next;
				mani->files = node_extract(node_mani, mani->files);
				char* fn = ((struct File*)(node_mani->data.ptr))->name;remove(fn);
				free_node(node_mani,free_file);
				mods->cnt--; mani->cnt--;
				node_mod = node_mod->next; node_mani = temp;
			}else{
				void* temp = node_mani->data.ptr;
				node_mani->data.ptr = node_mod->data.ptr;
				node_mod->data.ptr = temp;
				f_mod->ver++;
				node_mod = node_mod->next; node_mani = node_mani->next;
			}
		}
	}
	return 0;
}


/**
 * 	use a file with change record to change manifest
 * 	and delete other files
 */

int change_mani_with_file_recv(int srv_sock, char* mani, char* file){
	if(send_one_file(srv_sock, file)==0){printf("%s sent.\n",file);}//send req
	recv_proj(srv_sock);//recv file

	struct Project* mods = indexer_read(file);
	struct Project* old_manifest = indexer_read(mani);
	manifest_update(mods, old_manifest);//update manifest
	old_manifest->proj_version = (old_manifest->proj_version++);
	free_proj(mods);
	remove(file);remove(mani);
	manifest_write(old_manifest);
	free_proj(old_manifest);
	return 0;
}

/*
 *
 * recv a change file and send out its required files
 * only used for upgrade
 */
int change_mani_with_file_send(int sock){
	char* name = recv_one_file(sock);//recv a change file
	if(name == NULL){printf("failed to recv file %s.\n", name);}
	struct Project* upgrade_files = indexer_read(name);
	remove(name);free(name);remove("./.commit");
	send_files(upgrade_files, sock);
	return 0;
}


/*******************************Name_path_conversion************************************/
/**
 * convert a folder name to its path
 * need to free
 */
char* add_dot_slash(char* name){
	char* path = name;
	if(name[0] != '.' || name[1] != '/'){
		path = (char*)calloc((strlen(name)+ 2 + 1), sizeof(char));
		strcpy(path, "./");
		strcat(path, name);
	}else{
		char* path = (char*)calloc((strlen(name) + 1), sizeof(char));
		strcpy(path, name);
	}
	return path;
}

/**
 *
 * get rid of ./ from a give path if they have one
 * need to free
 */
char* strip_dot_slash(char* path){
	int result_len = 0; char* result = NULL;
	if(path[0] == '.' && path[1] == '/'){
		result_len = strlen(path) + 1 - 2;
		result = calloc((result_len), sizeof(char));
		strcpy(result, path+2);
	}else{
		result_len = strlen(path) + 1;
		result = calloc((result_len), sizeof(char));
		strcpy(result, path);
	}
	return result;
}


/**
 *
 * check if a directory exists
 * if not return -1
 *  else reutrn 0
 */
int dir_exist(char* path){
	struct stat st = {0};
	int result = stat(path, &st);
	return result;
}


/**
 *  go in dir, if does not exist create one
 */
int step_in_dir(char* path){
	if (dir_exist(path) == -1) {mkdir(path, 0777);}
	chdir(path);
	return 0;
}

/*
 * go into a project folder from root by its name
 * + -> add ? ->check exist
 * return -1 if desired action failed
 */
int project_stepin(char* proj, char action){
	char* path = add_dot_slash(proj);
	if(dir_exist(path) == -1){
		if(action == '?'){return -1;}
	}else{
		if(action == '+'){printf("Proj exists.\n");return -1;}
	}
	step_in_dir(path);
	free(path);
	return 0;
}



/**
 * function for pointer data


void print_node_struct File(union Data data){
	struct File* file = (struct File*)(data.ptr);
	printf("struct File: %s, Length: %d, Hash: %s\n", file->name, file->len, file->hash);
}

void free_data_struct File(union Data data){
	struct File* file = (struct File*)(data.ptr);
	free(file->hash);free(file->name);free(file);
}

int comp_struct File(union Data a, union Data b){
	char* a_n = ((struct File*)(a.ptr))->name;
	char* b_n = ((struct File*)(b.ptr))->name;
	return strcmp(a_n, b_n);
}
 */
