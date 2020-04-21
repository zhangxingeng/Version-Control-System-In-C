#include "msc.h"


/**
 *
 * get a token from a fd to a buff
 */
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
 *	parse a file from a block of data
 */
char* read_bytes(int sock, int len){
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
char* file_readin(char* path){
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
















