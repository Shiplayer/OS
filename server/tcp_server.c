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
#include <fcntl.h>

char* getName(char* path){
    int index = -1;
    printf("strlen(path) = %d\n", strlen(path));
    char name[256];
    memset(name, 0, 256);
    for(int i = strlen(path) - 1; i >= 0; i--){
        printf("%d\n", i);
        if(path[i] == '/'){
            index = i;
            break;
        }
    }
    if(index == -1){
        return path;
    }
    for(int i = index, j = 0; i < strlen(path); i++, j++){
        name[j] = path[i];
    }
    printf("strlen(path) = %d\n", strlen(path));
    
    return name;
}

int main(int argc, char* argv[]){
	int sd;
	int  new_sd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sin_size;
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket creation failed");
		exit(1);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = 3490;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero), 8);
	if(bind(sd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
		perror("socket bind failed");
		exit(1);
	}
	if(listen(sd, 10) == -1){
		perror("listen failed");
		exit(1);
	}
//	while(1){
		sin_size = sizeof(struct sockaddr_in);
		if((new_sd = accept(sd, (struct sockaddr*)&client_addr, (socklen_t*)&sin_size)) < 0){
                        perror("accept failed");
                        exit(1);
                }
		printf("server get connection from %s \n", inet_ntoa(client_addr.sin_addr));
/*		pid_t pid_0 = fork();
		switch(pid_0){
			case -1:{
				perror("fork failed");
				exit(1);
			}*/
//			case 0:{
				char buf[4096];
                FILE *fb = fopen(argv[1], "r");
                fseek(fb, 0, SEEK_END);
                long len = ftell(fb) + 1; 
//                uint64_t un = htonl(len); 
                rewind(fb);
                close(fb);
				int bytes_send = 0;
                if((bytes_send = send(new_sd, &len, sizeof(long), 0)) == -1){
                    perror("send failed");
                    exit(1);
                }
				printf("Send %d bytes, message: %d \n", bytes_send, len);
                int fb_n = open(argv[1], O_RDONLY);
                char *name = getName(argv[1]);
                printf("name of file: %s\n", name);
                int ssize = strlen(name);
                if((bytes_send = send(new_sd, ssize, sizeof(int), 0)) == -1){
                      perror("send failed");
                      exit(1);
                }
				printf("Send %d bytes, message: %d \n", bytes_send, len);
                if((bytes_send = send(new_sd, name, strlen(name), 0)) == -1){
                      perror("send failed");
                      exit(1);
                } 
				printf("Send %d bytes, message: %d \n", bytes_send, len);
                int buff;
                for(long i = 0; i < len; i += 4096){
                    size_t ss = read(fb_n, buf, 4096);
                    printf("file read(%d) \n",ss);
		    		if((bytes_send = send(new_sd, buf, ss, 0)) == -1){
	    				perror("send failed");
    					exit(1);
    				}
		    		printf("Send %d bytes(%d/%d), message: \n", bytes_send, i + bytes_send, len);

                    /*size_t srecv = recv(new_sd, &buff, sizeof(int), MSG_WAITALL);
				    printf("Recv %d bytes, message: %s \n", srecv, buff);
                    if(buff != 1){
                        exit(0);
                    }*/
                }
                printf("end\n");
				close(new_sd);
                printf("end\n");
				return 0;
//            }
/*			default:{
				wait(pid_0);
				//shutdown(new_sd, SHUT_RDWR);
				close(new_sd);
			}*/
//		}
//	}
	return 0;
}
