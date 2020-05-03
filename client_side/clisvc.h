#ifndef CLISVC_H
#define CLISVC_H


#include "libs/msc.h"
#include "libs/files_io.h"

int create_project_cli(char* proj, int srv_sock);

int add_a_file_cli(char* proj, char* file_name);
int delete_a_file_cli(char* proj, char* file_name);

int commit_cli(char* proj, int srv_sock);
int push_cli(char* proj, int srv_sock);
int update_cli(char* proj, int srv_sock);
int upgrade_cli(char* proj, int srv_sock);


char file_change_detect(struct Node* loco, struct Node* remo);


#endif
