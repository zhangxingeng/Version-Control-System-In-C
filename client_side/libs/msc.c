#include "msc.h"


int read_token(int sock, char* buff, int buff_size, char delim){
	memset(buff, '\0', buff_size*sizeof(char));
	int total = 0, cnt = 0;
	while(total < buff_size){
		cnt = read(sock, buff+total, 1);
		total += cnt;
		if(buff[total-1] == delim){
			buff[total-1] = '\0';
			break;
		}else if(cnt == 0){
			buff[total] = '\0';
			break;
		}
	}
	return 0;
}


/**
 *
 * read a command into a buffer
 */
char* read_cmd(int fd, char* buff, int buff_size){
	//read the len of the command
	memset(buff, '\0', buff_size*sizeof(char));
	read_token(fd, buff, buff_size, ' ');

	//read the actual command
	int cmd_len = atoi(buff); int total = 0, cnt = 0;
	memset(buff, '\0', buff_size*sizeof(char));
	while(total < cmd_len){
		cnt = read(fd, buff+total, cmd_len);
		total += cnt;
	}
	return buff;
}


/**
 * write command to socket
 */
int write_cmd(int fd, char* cmd){
	int stream_len = strlen(cmd)+int_len(strlen(cmd))+1;
	char* stream = (char*)malloc((stream_len+1)*sizeof(char));
	memset(stream, '\0', stream_len*sizeof(char));
	sprintf(stream, "%d", (int)strlen(cmd));
	strcat(stream, " ");strcat(stream, cmd);
	write(fd, stream, stream_len);
	free(stream);
	return 0;
}


/**
 *	parse a file from a block of data
 */
char* parse_file_from_readin(int sock, int len){
	char* buff = (char*)malloc((len+1)*sizeof(char));
	memset(buff, '\0', (len+1)*sizeof(char));
	int cnt = 0, total = 0;
	while(total != len){
		cnt = read(sock,(buff+total), (len-total) * sizeof(char));
		total += cnt;
	}
	return buff;
}


/**
 * 	turn a file to memory return as string
 */
char* read_file_to_mem(char* path){
	int filelen = file_len(path);
	int fd = open(path, O_RDONLY);
	char* mem = (char*)malloc((filelen+1)*sizeof(char));
	memset(mem, '\0', (filelen+1)*sizeof(char));
	read(fd, mem, filelen);
	close(fd);
	return mem;
}


/**
 * 	convert a buff to a string malloc
 *	need to be freed
 */
char* buff_to_str(char* buff, int buff_size){
	int str_len = strlen(buff);
	char* str = (char*)malloc((str_len+1)*sizeof(char));
	strcpy(str, buff);
	return str;
}


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



int dump_to_sock(char* msg, int fd){
	int cnt = 0, len = strlen(msg);
	while(cnt != len){
		cnt += write(fd, msg+cnt, len-cnt);
	}
	return cnt;
}








