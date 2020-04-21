#include "files_io.h"


/**
 * 	count the length of an int
 */
int int_len(int n){
	int cnt = 0;
	while (n != 0) {n /= 10; ++cnt;}
	return cnt;
}


int file_len(const char *file_name){
    struct stat st;
    if(stat(file_name,&st)==0)
        return (st.st_size);
    else
        return -1;
}


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
		char* file_content = file_readin(files.file_pathname[i]);
		strcat(stream, file_content);
		free(file_content);
	}
	int total_sent = send_str(stream, sock);
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
		get_token_from_recv(sock, buff, buff_size, ':');	//file name
		files->file_pathname[i] = buff_to_str(buff, buff_size);
		read_token(sock, buff, buff_size, ':');	//file length
		files->file_len[i] = atoi(buff);
	}
	for(int i = 0; i < files->cnt; i++){		//unpack files
		char* buff = read_bytes(sock, files->file_len[i]);
		int fd = open(files->file_pathname[i], O_WRONLY|O_CREAT, 0666);
		write(fd, buff, files->file_len[i]);
		free(buff);
		close(fd);
	}
	return files;
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
