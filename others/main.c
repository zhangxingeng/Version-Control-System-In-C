
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "socket_lib.h"



/*
int main(){
	int fp = open("./test", O_RDWR|O_CREAT, 0666);
	send_str("hello World\n", fp);
	char* got = recv_str(fp);
	free(got);
	close(fp);
}
*/


