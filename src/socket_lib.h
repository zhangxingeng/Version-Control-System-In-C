#ifndef SOCK_LIB_H
#define SOCK_LIB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#include <netdb.h>
#include <sys/socket.h>

int send_str(char* msg, int fd);
int recv_str(int fd, char* buff, int buff_size);
int conn_estab(char type, char* name, int port);

#endif
