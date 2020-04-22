#include "files_io.h"



/**
 * 	send files to recv socket
 */
int send_files(struct file_list files, int sock){
	long int total_byte = 10; //len of "sendfiles "
	total_byte += (int_len(files.cnt)+1); //<file_#>:
	for(int i = 0; i < files.cnt; i++){
		int file_name_len= strlen(files.file_pathname[i]);
		total_byte += (int_len(file_name_len)+1);//<file_name_len_len>:
		total_byte += (file_name_len+1);//<file_name_len>:
		total_byte += (int_len(files.file_len[i])+1); //<file_len_len>:
	}
	total_byte++;//for \0
	char* stream = (char*)malloc(total_byte * sizeof(char));
	memset(stream, '\0', total_byte * sizeof(char));
	strcpy(stream, "sendfiles ");
	char buff[40];
	sprintf(buff, "%d", files.cnt);
	strcat(stream, buff);strcat(stream, ":");
	for(int i = 0; i < files.cnt; i++){
		strcat(stream, files.file_pathname[i]); strcat(stream, ":");
		sprintf(buff, "%d", files.file_len[i]);
		strcat(stream, buff); strcat(stream, ":");
	}
	for(int i = 0; i < files.cnt; i++){
		char* file_content = read_file_to_mem(files.file_pathname[i]);
		strcat(stream, file_content);
		free(file_content);
	}
	int total_sent = dump_to_sock(stream, sock);
	free(stream);
	return total_sent;
}




/**
 * recv files from a socket
 */
struct file_list* recv_files(int sock){
	struct file_list* files = (struct file_list*)malloc(sizeof(struct file_list));
	int buff_size = 1000;
	char buff[buff_size];
	read_token(sock, buff, buff_size, ' '); //sendfiles command should move to cmd to func
	read_token(sock, buff, buff_size, ':');	//file number
	files->cnt = atoi(buff);
	files->file_pathname = (char**)malloc(files->cnt*sizeof(char*));
	files->file_len = (int*)malloc(files->cnt*sizeof(int));
	memset(files->file_pathname, '\0', files->cnt*sizeof(char*));
	for(int i = 0; i < files->cnt; i++){
		read_token(sock, buff, buff_size, ':');	//file name
		files->file_pathname[i] = buff_to_str(buff, buff_size);
		read_token(sock, buff, buff_size, ':');	//file length
		files->file_len[i] = atoi(buff);
	}
	for(int i = 0; i < files->cnt; i++){		//unpack files
		char* buff = parse_file_from_readin(sock, files->file_len[i]);
		int fd = open(files->file_pathname[i], O_WRONLY|O_CREAT, 0666);
		write(fd, buff, files->file_len[i]);
		free(buff);
		close(fd);
	}
	return files;
}


/**
 *  go in dir, if does not exist create one
 */
int step_in_dir(char* name){
	char* path = name;
	if(name[0] != '.' || name[1] != '/'){
		char* path = (char*)malloc((strlen(name)+ 2 + 1) * sizeof(char));
		strcpy(path, "./");strcat(path, name);
	}
	struct stat st = {0};
	if (stat(path, &st) == -1) {mkdir(path, 0777);}
	chdir(path);
	return 0;
}



/**
int main(){
	//this part is for recv file
	int fd = open("./final", O_RDONLY);
	struct file_list* files = recv_files(fd);
	close(fd);
	printf("%d", files->cnt);
	//this part is for testing sendfile
	char* pathname[] = {"./1", "./2", "./3"};
	int filelen[3] = {file_len(pathname[0]), file_len(pathname[1]), file_len(pathname[2])};
	struct file_list list;

	list.cnt = 3;
	list.file_pathname = pathname;
	list.file_len = filelen;
	remove("./final");
	int fd = open("./final", O_RDWR|O_CREAT, 0666);
	printf("Sent out bytes: %d", send_files(list, fd));
	close(fd);


}
*/
