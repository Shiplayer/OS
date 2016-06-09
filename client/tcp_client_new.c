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

	printf("Please, write your message \n");

	if(!strcmp(gets(message), "exit")){

		close(sd);

		exit(0);

	}

	int chars_send = 0;

	if((chars_send = send(sd, message, sizeof(message), 0)) == -1){

		perror("send failed");

		exit(1);

	}

	printf("Waiting for message \n");

	int bytes_recive = 0;

	bytes_recive = recv(sd, buf, sizeof(message), MSG_WAITALL);

	printf("Recive %d bytes message: %s \n", bytes_recive, buf);

	return 0;

}
