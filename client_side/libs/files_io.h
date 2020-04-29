#ifndef FILES_IO_H
#define FILES_IO_H

//#include <netinet/in.h>
//#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for read()
#include <fcntl.h>	//file related
#include <dirent.h>//for directory

#include <openssl/md5.h>//for md5
#include <sys/stat.h>
#include "socket_lib.h"
#include "msc.h"
#include "ll.h"
/**
 * send out a repo to a sock
 */
struct Node;
struct Project{
	char* proj_name;
	int proj_version;
	int cnt;
	struct Node* files;
};

struct File{
	char* name;
	int len;
	int ver;
	char* hash;
	char act; //madc
};

int send_files(struct Project* proj, int sock);
struct Project* recv_files(int sock);

int send_one_file(int sock, char* file_name);

int comp_file_name(union Data a, union Data b);
int delete_file_node(struct Project* proj, struct Node* node);

void free_file(union Data file);
void free_proj(struct Project* proj);;

struct Project* manifest_read(char* name);
int manifest_write(struct Project* proj);

char* add_dot_slash(char* name);//need to free
char* strip_dot_slash(char* path); //need to free

int dir_exist(char* path);
int step_in_dir(char* path);
int project_stepin(char* proj, char action);


#endif













