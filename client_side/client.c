
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "libs/socket_lib.h"
#include "clisvc.h"


/**
 *  convert user command into function
 */
int cmd_to_svc(int cli_sock, char* cmd, char* proj, char* arg1){
	int result = 0;

	if(strcmp(cmd, "create") == 0){
		if(project_stepin(proj, '?') != -1){return -1;}
		result = create_project_cli(proj, cli_sock);
	}else{
		if(project_stepin(proj, '?') != 0){return -1;}
		if(strcmp(cmd, "add") == 0){
			result = add_a_file_cli(proj, arg1);
		}else if (strcmp(cmd, "delete") == 0){
			result = delete_a_file_cli(proj, arg1);
		}else if(strcmp(cmd, "update") == 0){
			result = update_cli(proj, cli_sock);
		}else if(strcmp(cmd, "upgrade") == 0){
			result = upgrade_cli(proj, cli_sock);
		}else if(strcmp(cmd, "commit") == 0){
			result = commit_cli(proj, cli_sock);
		}else if(strcmp(cmd, "push") == 0){
			result = push_cli(proj, cli_sock);
		}else{
			result = -1;
		}
	}
	return result;
}



int main(int argc, char** argv){
	int sock_fd = conn_estab('c', "localhost", 10035);
	printf("%d", cmd_to_svc(sock_fd, "create", "kuma", NULL));
	//printf("%d", cmd_to_svc(sock_fd, argv[1], argv[2], NULL));
	close(sock_fd);
}


/*/
int main(){

	//this part is for testing sendfile


	struct File file1 = {"./1", file_len("./1"), 0, NULL};
	struct File file2 = {"./2", file_len("./2"), 0, NULL};
	struct File file3 = {"./3", file_len("./3"), 0, NULL};

	struct Node nd3;nd3.data.ptr = (void*)&file3;nd3.next = NULL;
	struct Node nd2;nd2.data.ptr = (void*)&file2;nd2.next = &nd3;
	struct Node nd1;nd1.data.ptr = (void*)&file1;nd1.next = &nd2;

	struct Project proj = {"test", 0, 3, &nd1};


	remove("./final");
	int fd = open("./final", O_RDWR|O_CREAT, 0666);
	printf("Sent out bytes: %d", send_files(&proj, fd));
	close(fd);


	int fd = open("./final", O_RDONLY);
	struct Project* proj2;
	proj2 = recv_files(fd);
	free(proj2);
	close(fd);



}
*/
