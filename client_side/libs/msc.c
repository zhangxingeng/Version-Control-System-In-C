#include "msc.h"


int read_token(int sock, char* buff, int buff_size, char delim){
	memset(buff, '\0', buff_size*sizeof(char));
	int total = 0, cnt = 0, retry = 0;
	while(total < buff_size){
		cnt = read(sock, buff+total, 1);
		total += cnt;
		if(total < 1){
			if(retry >= RETRY){return 0;}
			retry++;
		}else if(buff[total-1] == delim){
			buff[total-1] = '\0';
			break;
		}else if(cnt == 0){
			buff[total] = '\0';
			break;
		}
	}
	return total;
}

int write_str(int sock, char* str, int str_len){
	int total = 0, cnt = 0, retry = 0;
	while(total < str_len){
		cnt = write(sock, str+total, str_len-total);
		if(cnt == 0){
			if(retry >= RETRY){printf("write err.\n"); return 0;}
			retry++;
		}
		total += cnt;
	}
	return total;
}


int read_str(int sock, char* buff, int buff_size, int read_len){
	int total = 0, cnt = 0, retry = 0;
	while(total < read_len){
		cnt = read(sock, buff+total, read_len);
		if(cnt == 0){
			if(retry >= RETRY){printf("write err.\n"); return 0;}
			retry++;
		}
		total += cnt;
	}
	return total;
}
/**
 *
 * read a command into a buffer
 * cmd format: <len_cmd> <cmd>
 *
 */
int read_cmd(int fd, char* buff, int buff_size){
	memset(buff, '\0', buff_size*sizeof(char));
	read_token(fd, buff, buff_size, ' ');
	int cmd_len = atoi(buff);
	if(read_str(fd, buff, buff_size, cmd_len)<=0){return -1;}
	return 0;
}


/**
 * write command to socket
 */
int write_cmd(int fd, char* cmd){
	int stream_len = strlen(cmd)+int_len(strlen(cmd))+1;
	char* stream = calloc((stream_len+1), sizeof(char));
	sprintf(stream, "%d", (int)strlen(cmd));
	strcat(stream, " ");strcat(stream, cmd);
	write_str(fd, stream, stream_len);
	free(stream);
	return 0;
}

/**
 *	parse a file from a block of data
 */
char* parse_file_from_readin(int sock, int len){
	char* buff = calloc((len+1), sizeof(char));
	if(read_str(sock, buff, len+1, len)!=0){printf("Read in failed.\n");}
	return buff;
}


/**
 * 	turn a file to memory return as string
 */
char* read_file_to_mem(char* path){
	int filelen = file_len(path);
	int fd = open(path, O_RDONLY);
	char* mem = calloc((filelen+1), sizeof(char));
	read_str(fd, mem, filelen+1, filelen);
	close(fd);
	return mem;
}


/**
 * 	convert a buff to a string malloc
 *	need to be freed
 */
char* buff_to_str(char* buff, int buff_size){
	int str_len = strlen(buff);
	char* str = calloc((str_len+1), sizeof(char));
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
        return 0;
}


/**
 * convert an unsigned char array to a hexadecimal string
 * need to free
 */
char* u_char_to_str(unsigned char* arr, int arr_len){
	char* result = calloc((arr_len*2+1), sizeof(char));
	int x = 0;
	for(; x < arr_len; x++)
	    sprintf(result+(x*2), "%02x", arr[x]);
	result[arr_len*2] = '\0';
	return result;
}


/**
 *  generate md5 for a file
 *  need to free
 */
char* gen_md5(char* file_name){
	int fd = open(file_name, O_RDONLY);
	if (fd <= 0) {printf ("struct File open err.\n");return NULL;}

	unsigned char raw_md5[MD5_DIGEST_LENGTH];//16
	MD5_CTX mdContext;
	int read_num;
	unsigned char buff[1024];

	MD5_Init (&mdContext);
	while ((read_num = read(fd, buff, 1024)) > 0){
		MD5_Update (&mdContext, buff, read_num);
	}
	MD5_Final (raw_md5,&mdContext);
	char* result = u_char_to_str(raw_md5, MD5_DIGEST_LENGTH);

	close (fd);
	return result;
}

int cmd_relay(char* proj, char* cmd, int sock){
	char msg[1024];
	strcpy(msg, cmd);strcat(msg, " ");
	strcat(msg, proj);
	return write_cmd(sock, msg);
}

/*
 * check if a string is a number
 *
 */
int str_is_number(char* str){
	int i = 0;
	while(str[i] != '\0'){
		if(str[i] < '0' || str[i] > '9'){return 0;}
		i++;
	}
	return 1;
}














