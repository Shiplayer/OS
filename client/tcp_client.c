#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

int main(int argc, char **argv){
	char message[4096];
	char buf[4096];
	int port, ch;
	int sd;
	struct sockaddr_in addr;
	struct hostent* hostinfo;
	if(argc != 3){
		printf("few arguments \n");
		exit(0);
	}
	port = atoi(argv[1]);
	hostinfo = argv[2];
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket creation failed");
		exit(1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr(hostinfo);
	printf("port: %d \n" , port);
	printf("hostinfo: %s \n", hostinfo);
	bzero(&(addr.sin_zero), 8);
	if(connect(sd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1){
		perror("connect failed");
		exit(1);
	}
	printf("Waiting for size \n");
	long bytes_need = 0;
	recv(sd, &bytes_need, sizeof(long), 0);
	printf("Size is: %d \n", bytes_need);
	int bytes_recive = 0;
	int total_bytes = 0;
	int size_of_name_of_file = 0;
	recv(sd, &size_of_name_of_file, sizeof(int), 0);
	printf("size of name of file %d \n", size_of_name_of_file);
	char name_of_file[4096];
	recv(sd, name_of_file, sizeof(char)*size_of_name_of_file,0);
	printf(" name of file: %s \n", name_of_file);
	int fd_1 = open(name_of_file , O_WRONLY|O_CREAT, 0777);
	for(;;){
		bytes_recive = recv(sd, &buf, sizeof(message), 0);
		total_bytes += bytes_recive;
		printf("Bytes recive: %d ,Total progress: %d/%d \n ", bytes_recive, total_bytes, bytes_need);
//		int one = 1;
//		send(sd, one, sizeof(int), 0);
//		printf("1 \n");
		write(fd_1, buf, bytes_recive);
		if (bytes_recive == 0){
			break;
		}
	}
	close(fd_1);
	printf("Recive %d \n", total_bytes);
	return 0;
}
