#ifndef SRVSVC_H
#define SRVSVC_H

#include <fcntl.h>	//file related

#include "libs/files_io.h"
#include "libs/msc.h"


int cmd_to_svc(int cli_sock, char* cmd, char* proj);
int create_project_srv(char* proj, int cli_sock);










#endif
