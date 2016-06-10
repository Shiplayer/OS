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
#include <signal.h>

int main(int argc, char **argv){
	char message[4096];
	char buf[4096];
	int port, ch;
	int sd, new_sd;
	struct sockaddr_in addr;
	struct hostent* hostinfo;
	if(argc != 3){
		printf("few arguments \n");
		exit(0);
	}
	port = atoi(argv[1]);
	hostinfo = argv[2];
	if((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("socket creation failed");
		exit(1);
	}
	memset(&addr, 0 , sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr(hostinfo);
	printf("port: %d \n" , port);
	printf("hostinfo: %s \n", hostinfo);
	while(1){
		printf("Please, write your message \n");
		if(!strcmp(gets(message), "exit")){
			close(sd);
			exit(0);
		}
		new_sd = sd;
		int chars_send = 0;
		int clolen = sizeof(addr);
		if((chars_send = sendto(new_sd, message, 100, 0, (struct sockaddr*)&addr, clolen)) == -1){
			perror("send failed");
			exit(1);
		}
		printf("Chars sended: %d \n", chars_send);
		printf("Waiting for message \n");
		int pipefd[2];
		if(pipe(pipefd) == -1){
			perror("pipe");
			exit(1);
		}
		pid_t pid_0 = fork();
		switch(pid_0){
			case -1:{
				perror("fork failed");
				exit(1);
			}
			case 0:{
				if(recvfrom(new_sd, (void*)buf, 100, 0, (struct sockaddr*)&addr, &clolen) < 0){
					perror("recvfrom failed");
					exit(1);
				}
				printf("Message: %s \n", buf);
				exit(0);
			}
			default:{
				sleep(1);
				kill(pid_0, SIGKILL);
			}
		}
	}
	close(sd);
	return 0;
}
