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
#include <arpa/inet.h>
#include <ctype.h>

int main(){
	printf("Server started \n");
	int sd;
	int  new_sd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sin_size;
	if((sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		perror("socket creation failed");
		exit(1);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = 3490;
	if(bind(sd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
		perror("socket bind failed");
		exit(1);
	}
	memset(&client_addr, 0, sizeof(client_addr));
	int clilen = sizeof(client_addr);
	while(1){
		char buf[4096];
		new_sd = sd;
		if((sin_size = recvfrom(new_sd, (void*)&buf, 100, 0, (struct sockaddr*)&client_addr, &clilen)) < 0){
			perror("recvfrom failed");
			close(sd);
			exit(1);
		}
		printf("Bytes recive: %d , message: %s \n", sin_size , buf);
		int i = 0;
		for(i = 0; i < strlen(buf); ++i){
			buf[i] = toupper(buf[i]);
		}
		printf("sended %s \n", buf);
		if(sendto(new_sd, (void*)buf, 100 , 0, (struct sockaddr*)&client_addr, clilen) < 0){
			perror("Error in sendto");
			close(sd);
			exit(1);
		}
	}
	return 0;
}
