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
	total_byte += (int_len(proj->cnt)+1); //<file_#>:
	struct Node* ptr = proj->files;
	struct File* curr_file = NULL;

	while(ptr != NULL){
		curr_file = (struct File*)(ptr->data.ptr);
		int file_name_len= strlen(curr_file->name);
		total_byte += (file_name_len+1);//<file_name_len>:
		total_byte += (int_len(curr_file->len)+1); //<file_len_len>:
		total_byte += file_len(curr_file->name);//file_len
		ptr = ptr->next;
	}
	total_byte++;//for \0
	printf("memory malloced: %ld\n",total_byte);

	char* stream = (char*)calloc(total_byte, sizeof(char));
	strcpy(stream, "sendfiles ");
	char buff[40];
	sprintf(buff, "%d", proj->cnt);
	strcat(stream, buff);strcat(stream, ":");

	ptr = proj->files;
	while(ptr != NULL){
		curr_file = (struct File*)(ptr->data.ptr);
		strcat(stream, curr_file->name); strcat(stream, ":");
		sprintf(buff, "%d", curr_file->len);
		strcat(stream, buff); strcat(stream, ":");
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
 */
struct Project* recv_files(int sock){
	struct Project* proj = calloc(1, sizeof(struct Project));
	int buff_size = 1000;
	char buff[buff_size];
	if(read_token(sock, buff, buff_size, ' ') <=0){free_proj(proj); return NULL;}
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

		if(prev == NULL){proj->files = curr;}
		else{prev->next = curr;}
		prev = curr;
	}

	curr = proj->files;
	char* file_buff = NULL;
	while(curr != NULL){		//unpack files
		curr_file = (struct File*)(curr->data.ptr);
		file_buff = parse_file_from_readin(sock, curr_file->len);
		if(dir_exist(curr_file->name)==0){		//when file already exists
			if(strcmp(curr_file->name, "./.Manifest") == 0){
				curr_file->name = "./.manifest_remote";
			}else if(strcmp(curr_file->name, "./.commit") == 0){
				//to be done when doing commit
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
 */
struct Project* manifest_read(char* name){
	if(dir_exist(name) == -1){return NULL;}
	struct Project* proj = calloc(1, sizeof(struct Project));
	int fd = open(name, O_RDONLY);

	int buff_size = 1000;
	char buff[buff_size];
	read_token(fd, buff, buff_size, '\n');proj->proj_version = atoi(buff);
	struct Node* prev = NULL, *curr_node = NULL;
	struct File* curr_file = NULL;
	while(1){
		if(read_token(fd, buff, buff_size, ' ') <= 0){break;}
		curr_node = calloc(1, sizeof(struct Node));
		curr_file = calloc(1,sizeof(struct File));
		curr_node->data.ptr = curr_file;//link file to node
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
	sprintf(buff, "%d\n", proj->proj_version); write_str(fd, buff, strlen(buff));
	struct Node* n_ptr = proj->files; struct File* f_ptr = NULL;
	while(n_ptr != NULL){
		f_ptr = (struct File*)(n_ptr->data.ptr);
		sprintf(buff, "%d ", f_ptr->ver); write_str(fd, buff, strlen(buff));
		write_str(fd, f_ptr->name, strlen(f_ptr->name)); write_str(fd," ",strlen(" "));
		write_str(fd, f_ptr->hash, strlen(f_ptr->hash)); write_str(fd,"\n",strlen("\n"));
		n_ptr = n_ptr->next;
	}
	free_proj(proj);
	close(fd);
	return 0;
}


/**
 *  convert project to manifest
 */




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
