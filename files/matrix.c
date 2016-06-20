#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

int main(int argc, char* argv[]){
	srand(time(NULL));
	int n = 0;
	int m = 0;
	scanf("%d", &n);
	scanf("%d", &m);
	int shared_segment_size = (3*n*m+1)*getpagesize();
	int segment_id = shmget(IPC_PRIVATE,shared_segment_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if(segment_id == -1){
		perror("failed to shmget");
		exit(1);
	}
	int* shared_memory = (int*) shmat(segment_id,0,0);
	shared_memory[3*m*n] = 0;
	pid_t pid_0 = fork();
	if(pid_0 == -1){
		perror("fork failed");
		exit(1);
	} else if(pid_0 == 0) {
		while(1){
			if(shared_memory[3*m*n] == 1){
				exit(0);
			}
			if(shared_memory[3*m*n] != 2)
				continue;

			for(int i = 0; i < n; ++i){
				for(int j = 0; j < m; ++j){
					shared_memory[2*m*n+i*m+j] = shared_memory[i*m+j] + shared_memory[m*n+i*m+j];
				}
			}
			shared_memory[3*n*m] = 3;
		}
	} else {
		pid_t pid_1 = fork();
		if(pid_1 == -1){
			perror("fork failed");
			exit(1);
		} else if(pid_1 == 0) {
			while(1){
				if(shared_memory[3*n*m] == 1){
					exit(0);
				}
				if(shared_memory[3*n*m] != 0){
					continue;
				}
				printf("read one matrix\n");
				for(int i = 0; i < n; ++i){
					for(int j = 0; j < m; ++j){
						int buf = rand()%50;
						printf("%d ", buf);
						shared_memory[i*m+j] = buf;
					}
					printf("\n");
				}
				printf("read other matrix\n");
				for(int i = 0; i < n; ++i){
					for(int j = 0; j < m; ++j){
						int buf = rand()%50;
						printf("%d ", buf);
						shared_memory[i*m+j+m*n] = buf;
                    }
					printf("\n");
				}
				shared_memory[3*n*m] = 2;
			}
		}
		else {
			while(1){
				if(shared_memory[3*n*m] != 3){
					continue;
				}
				printf("result:\n");
				for(int i = 0; i < n; ++i){
            		for(int j = 0; j < m; ++j){
                    	printf("%d ", shared_memory[2*m*n+i*m+j]);
            		}
                    printf(" \n");
                }
				printf("\n\n");
				if(getchar() == 'q'){
					shared_memory[m*n*3] = 1;
					wait(pid_0);
					wait(pid_1);
					shmdt(shared_memory);
					if(shmctl(segment_id, IPC_RMID, 0) == -1){
						perror("shmctl failed");
						exit(1);
					}
					return 0;
				}
				shared_memory[3*m*n] = 0;
			}
		}
	}
	return 0;
}
