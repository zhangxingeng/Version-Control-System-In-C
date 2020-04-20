#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#define _GNU_SOURCE

char* send_files (char* projectPath, char* fullPath);

int main (int argc, char* argv[]) {
	char* pP = argv[1];
	char* fP = argv[2];
	char* one = send_files(pP, fP);
	printf("%s\n", one);

	return 0;
}

char* send_files (char* projectPath, char* fullPath) {
	//project name > folders/filename>./folders/filename>read file into memory
	// projectPath = ./project
	// fullPath = project/src/test
	// get file name from fullPath
	int fd = open(fullPath, O_RDONLY);
	int length = lseek(fd, 0, SEEK_END);
	void* str = mmap(0, length, PROT_READ, MAP_PRIVATE, fd, 0);
	//printf("%s\n%d\n", str, length);
	int len = strlen(fullPath);

	int numFiles = 1;

	//int a = length + 8 + len;
	char* string = (char*) malloc(200 * sizeof(char));
	strcat(string, "sendfile");
	strcat(string, ":");
	char* buffer = (char*) malloc(5 * sizeof(char));
	sprintf(buffer, "%d", numFiles);
	strcat(string, buffer);
	strcat(string, ":");
	sprintf(buffer, "%d", len);
	strcat(string, buffer);
	strcat(string, ":");
	strcat(string, fullPath);
	sprintf(buffer, "%d", numFiles);
	strcat(string, buffer);
	strcat(string, ":");
	strcat(string, str);
	//printf("str: %s\n", string);

	return string;
}