#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>


key_t key;
size_t size;
key_t key2;
size_t size2;

int main(){
	key = ftok("matrix", 1);
	key = ftok("matrix2", 2);
	pid_t pid;
	while(1){
		if((pid = fork()) == 0){
			int m, n;
			scanf("%d %d", &m, &n);
			int *matrix = malloc(m * n * sizeof(int));
			for(int i = 0; i < m; i++){
				for(int j = 0; j < n; j++)
					scanf("%d", matrix[i + j * m]);
			}
			size = m * n * sizeof(int);
			int shm_number = shmget(key, size, IPC_CREAT | 0666);
			if(shm_number < 0) {
				perror("shmget");
				exit(1);
			}
			int *shm = shmat(shm_number, NULL, 0);
			if(shm == (int *) -1){
				perror("shmat");
				exit(1);
			}
			int* s = shm;
			for(int i = 0; i < m; i++){
				for(int j = 0; j < n; j++){
					*s++ = matrix[i + j * m];
				}
			}
			scanf("%d %d", &m, &n);
			size2 = m * n * sizeof(int);
			int shm_number2 = shmget(key, size, IPC_CREAT | 0666);
			if(shm_number2 < 0){
				perror("shmget2");
				exit(1);
			}
			int *shm2 = shmat(shm_number, NULL, 0);
			if(shm2 == (int *) -1){
				perror("shmat2");
				exit(1);
			}
			int* s = shm2;
			for(int i = 0; i < m; i++){
				for(int j = 0; j < n; j++){
					*s++ = matrix[i + j * m];
				}
			}
			pid = fork();
			if(pid == 0){
				int 
			} else if(pid != -1)
				exit(0);
			else{
				perror("fork2");
				exit(1);
			}
		} else if(pid != -1){
			wait(1);
		} else{
			perror("fork");
			exit(1);
		}
	}
	return 0;
}

/*int main(){
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
			printf("%d\n", *s);
		*shm = -1;
		printf("exit (child)\n");
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
		printf("shmat (parent)\n");
		int *s = shm;
		scanf("%d", &(*s++));
		//*s++ = 10;
		*s = NULL;
		printf("wait (parent)\n");
		while(*shm != -1)
			sleep(1);

	} else{
		perror("pid");
		exit(1);
	}
	exit(0);
}*/
