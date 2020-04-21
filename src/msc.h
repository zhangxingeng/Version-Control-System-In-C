#ifndef MSC_H
#define MSC_H

#include <fcntl.h>

int read_token(int sock, char* buff, int buff_size, char delim);
char* read_bytes(int sock, int len);
char* file_readin(char* path);
char* buff_to_str(char* buff, int buff_size);


#endif
