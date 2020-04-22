#include "srvsvc.h"




/**
 * 	create a project at certain path
 *
 */
int create_project_srv(char* proj, int cli_sock){
	step_in_dir(proj);
	int fp = open("./.manifest", O_RDWR|O_CREAT, 0666);
	write(fp, "0\n", 2);//initialize version to be 0
	struct file_list files;
	files.cnt = 1;
	char* file_list[1] = {"./.manifest"};
	files.file_pathname = file_list;
	int manifest_len = file_len("./.manifest");
	int file_len[1] = {manifest_len};
	files.file_len = file_len;
	send_files(files, cli_sock);
	return 0;
}


/**
 * 	convert command from user to function on server
 *
 */
int cmd_to_svc(int cli_sock, char* cmd, char* proj){
	int result = 0;

	if(strcmp(cmd, "create") == 0){		//for create command
		result = create_project_srv(proj, cli_sock);
		if(result != 0){
			printf("got some error when creating Project.");
		}
	}else if(strcmp(cmd, "upgrade") == 0){			//other commands

	}else if(strcmp(cmd, "pull") == 0){			//other commands

	}else if(strcmp(cmd, "create") == 0){			//other commands

	}else if(strcmp(cmd, "create") == 0){			//other commands

	}else if(strcmp(cmd, "create") == 0){			//other commands

	}else{
		printf("command not recognized.\n");
		result = -1;
	}
	return result;
}






















