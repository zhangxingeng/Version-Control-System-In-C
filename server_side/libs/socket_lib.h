#ifndef SOCK_LIB_H
#define SOCK_LIB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#include <netdb.h>
#include <sys/socket.h>

int conn_estab(char type, char* name, int port);

#endif
