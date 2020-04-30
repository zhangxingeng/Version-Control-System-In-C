#ifndef MSC_H
#define MSC_H

#include <fcntl.h>
#include <string.h>	//memset
#include <stdlib.h>  //malloc
#include <unistd.h>	//read
#include <sys/stat.h>  //stat
#include <stdio.h>//sprintf

#include <openssl/md5.h>//  -lssl -lcrypto

#define RETRY 3 //retry time before error r/w

int read_token(int sock, char* buff, int buff_size, char delim);
int write_str(int sock, char* str, int str_len);

int read_cmd(int fd, char* buff, int buff_size);
int write_cmd(int fd, char* cmd);

char* parse_file_from_readin(int sock, int len);
char* read_file_to_mem(char* path);
char* buff_to_str(char* buff, int buff_size);

int int_len(int n);
int file_len(const char *file_name);

char* gen_md5(char* file_name); //need to free
int cmd_relay(char* proj, char* cmd, int sock);

int str_is_number(char* str);

#endif

