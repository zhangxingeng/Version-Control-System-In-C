#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for NULL
#include <fcntl.h>	//file related
#include <dirent.h>//for directory

//#include <sys/types.h>
#include <sys/stat.h>

#include "socket_lib.h"


/**
 * send out a repo to a sock
 */
struct file_list{
	char* proj_name;
	int proj_version;
	int cnt;
	int* file_ver;
	char** file_pathname;
	int* file_len;
	char** file_hash;
};



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
 * 	turn a file to memory return as string
 */
char* read_file(char* path){
	int filelen = file_len(path);
	int fd = open(path, O_RDONLY);
	char* mem = (char*)malloc((filelen+1)*sizeof(char));
	memset(mem, '\0', (filelen+1)*sizeof(char));
	read(fd, mem, filelen);
	close(fd);
	return mem;
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
		sprintf(buff, "%d", (int)strlen(files.file_pathname[i]));
		strcat(stream, buff); strcat(stream, ":");
		sprintf(buff, "%d", files.file_len[i]);
		strcat(stream, files.file_pathname[i]); strcat(stream, ":");
		strcat(stream, buff); strcat(stream, ":");
	}
	for(int i = 0; i < files.cnt; i++){
		char* file_content = read_file(files.file_pathname[i]);
		strcat(stream, file_content);
		free(file_content);
	}
	int total_sent = send_str(stream, sock);
	free(stream);
	return total_sent;
}









int main(){

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












