#ifndef SRVSVC_H
#define SRVSVC_H

#include <fcntl.h>	//file related

#include "libs/files_io.h"
#include "libs/msc.h"
#include "libs/ll.h"


int create_project_srv(char* proj, int cli_sock);
int commit_srv(char* proj, int cli_sock);
int push_srv(char* proj, int cli_sock);
int update_srv(char* proj, int cli_sock);
int upgrade_srv(char* proj, int cli_sock);









#endif
