#ifndef FILES_IO_H
#define FILES_IO_H

#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for NULL
#include <fcntl.h>	//file related
#include <dirent.h>//for directory

#include <sys/stat.h>
#include "socket_lib.h"
#include "msc.h"

/**
 * send out a repo to a sock
 */
struct file_list{
	char* proj_name;
	int proj_version;
	int cnt;		//Mandatory
	int* file_ver;
	char** file_pathname;//Mandatory
	int* file_len;//Mandatory
	char** file_hash;
};

int send_files(struct file_list files, int sock);
struct file_list* recv_files(int sock);
int step_in_dir(char* name);




#endif
