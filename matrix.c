#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>


key_t key;
size_t size = sizeof(int);

int main(){
	pid_t pid = fork();
	if(pid == 0){
		printf("child\n");
		int shm_number = shmget(key, size, 0666);
		if(shm_number < 0){
			perror("shm");
			exit(1);
		}
		printf("shmget is executed\n");
		int *shm = (int *)shmat(shm_number, NULL, 0);
		if(shm < 0){
			perror("shmat");
			exit(1);
		}
		printf("shmat is executed\n");
		for(int *s = shm; *s != NULL; s++)
			printf("%d", *s);
		*shm = -1;
		printf("exit (child)");
	} else if(pid != -1){
		printf("parent\n");
		int shm_number = shmget(key, size, IPC_CREAT |  0666);
		if(shm_number < 0){
			perror("shm parent");
			exit(1);
		}
		printf("shmget (parent)\n");
		int *shm = (int *)shmat(shm_number, NULL, 0);
		if(shm == -1){
			perror("shmat parent");
			exit(1);
		}
		printf("shmat (parent)");
		int *s = shm;
		*s++ = 10;
		*s = NULL;
		printf("wait (parent)");
		while(*shm != -1)
			sleep(1);

	} else{
		perror("pid");
		exit(1);
	}
	exit(0);
}
