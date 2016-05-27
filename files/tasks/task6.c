#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/ipc.h>
#include <pthread.h>

void th_read(int *argv){
	while(1){
		if(argv[2 + 3 * argv[1] * argv[0]] == 1){
			exit(0);
		}
		if(argv[3 * argv[1] * argv[0] + 4] != 0){
			continue;
		}
		for(int i = 0; i < argv[0]; ++i){
			for(int j = 0; j < argv[1]; ++j){
				argv[2 + i * argv[1] + j] = rand() % 99;
			}
		}
		for(int i = 0; i < argv[0]; ++i){
			for(int j = 0; j < argv[1]; ++j){
				argv[2 + i * argv[1] + j + argv[1] * argv[0]] = rand() % 99;
			}
		}
		argv[3 * argv[1] * argv[0] + 4] = 1;
	}
}

void th_summ(int *argv){
	while(1){
		if(argv[2 + 3 * argv[1] * argv[0]] == 1){
			exit(0);
		}
		if(argv[3 * argv[1] * argv[0] + 4] != 1){
			continue;
		}
		for(int i = 0; i < argv[0]; ++i){
			for(int j = 0; j < argv[1]; ++j){
				argv[2+2*argv[1]*argv[0]+i*argv[1]+j] = argv[2+i*argv[1]+j]+argv[2+argv[1]*argv[0]+i*argv[1]+j];
			}
		}
		argv[3 * argv[1] * argv[0] + 4] = 2;
	}
}

void th_write(int *argv){
	while(1){
		if(argv[2 + 3 * argv[1] * argv[0]] == 1){
			exit(0);
		}
		if(argv[3 * argv[1] * argv[0] + 4] != 2){
			continue;
		}
		for(int i = 0; i < argv[0]; ++i){
			for(int j = 0; j < argv[1]; ++j){
				printf("%d ", argv[2+i*argv[1]+j]);
			}
			printf("\n");
		}
		printf("\n");
		for(int i = 0; i < argv[0]; ++i){
			for(int j = 0; j < argv[1]; ++j){
				printf("%d ", argv[2+argv[1]*argv[0]+i*argv[1]+j]);
			}
			printf("\n");
		}
		printf("\n");
		for(int i = 0; i < argv[0]; ++i){
			for(int j = 0; j < argv[1]; ++j){
				printf("%d ", argv[2+2*argv[1]*argv[0]+i*argv[1]+j]);
			}
			printf("\n");
		}
		printf("\n\n");
		if(getchar() == 'q'){
			argv[2 + argv[1] * argv[0] * 3] = 1;
			return 0;
		}
		else{
			argv[3 * argv[1] * argv[0] + 4] = 0;
		}
	}
}

int main(int argc, char* argv[]){
		int seed = atoi(argv[1]);
		srand(seed);
		int n = rand() % 10 + 1;
		int m = rand() % 10 + 1;
		int* matrix_buffer = (int*)malloc((3*n*m+5)*sizeof(int));
		matrix_buffer[0] = n;
		matrix_buffer[1] = m;
		matrix_buffer[2 + 3 * m * n] = 0;
		matrix_buffer[3 * m * n + 3] == argv[1];  //dodelat
		matrix_buffer[3 * m * n + 4] == 0;
		pthread_t thread_0;
		if(pthread_create(&thread_0, NULL, th_read, matrix_buffer) != 0){
			perror("thread for read creation failed");
			exit(1);
		}
		pthread_t thread_1;
		if(pthread_create(&thread_1, NULL, th_summ, matrix_buffer) != 0){
                        perror("thread for summ creation failed");
                        exit(1);
                }
		pthread_t thread_2;
		if(pthread_create(&thread_2, NULL, th_write, matrix_buffer) != 0){
			perror("thread for write creation failed");
			exit(1);
		}
		if(pthread_join(thread_2, NULL) != 0){
			perror("join write failed");
			exit(1);
		}
		if(pthread_join(thread_1, NULL) != 0){
			perror("join summ failed");
			exit(1);
		}
		if(pthread_join(thread_0, NULL) != 0){
			perror("join read failed");
			exit(1);
		}
	return 0;
}
