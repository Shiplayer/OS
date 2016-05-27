#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <pthread.h>

void dfs(int *arc){
	int n = (int)arc[0];
	int i = 0;
	int j = 0;
	int **grap = (int **)malloc(n*sizeof(int*));
        for(i = 0; i < n; ++i){
                grap[i] = (int *)malloc(n*sizeof(int));
        }
	for(i = 0; i < n; ++i){
		for(j = 0; j < n; ++j){
			grap[i][j] =(int) arc[i*n + j + 1];
		}
	}
	int* used = (int *)malloc(n*sizeof(int));
        for(i = 0; i < n; ++i){
                used[i] = (int)arc[1 + n * n + i];
        }
	int v = (int)arc[1 + n * n + n];
	if(used[v] == 1){
		exit(0);
	}
	used[v] = 1; printf("we in %d \n", v);
	arc[1 + n * n + v] = 1;
	j = 0;
	for(i = 0; i < n; ++i){
		if((grap[v][i] != -1) & (used[i] == 0)){
		//	printf("used[%d] == %d \n", i, used[i]);
			j++;
			printf("from %d to %d \n", v, i);
			int s_err_thread = 0;
			pthread_t thread;
			int* dfs_in = (int *)malloc((3 + n * n + n)*sizeof(int));
			dfs_in[0] = n;
			int u = 0;
			for(u = 0; u < n; ++u){
				int o = 0;
				for(o = 0; o < n; ++o){
					dfs_in[1 + n * u + o] = grap[u][o];
				}
			}
			for(u = 0; u < n; ++u){
				dfs_in[1 + n * n + u] = used[u];
			}
			dfs_in[1 + n * n + n] = i;
			for(u = 0; u < 2 + (n*n) +n; ++u) printf("%d ", dfs_in[u]);
			printf("\n");
			s_err_thread = pthread_create(&thread, NULL, dfs, dfs_in);
			if(s_err_thread != 0){
				printf("%d %d \n", v, i);
				perror("error in thread create");
				exit(1);
			}
		//	int* dfs_out = (int *)malloc((n + 1)*sizeof(int));
			int join_stat = 0;
			join_stat = pthread_join(thread, NULL);
			if(join_stat != 0){
                                perror("join failed");
                                exit(1);
                        }
			printf("we in %d after dfs \n", v);
	//		printf("j == %d from dfs_out \n", dfs_out[0]);
			for(u = 0; u < n; ++u){
				if(used[u] == 0){
					used[u] = dfs_in[1 + n * n + u];
					arc[1 + n * n + u] = used[u];
				}
			}
			for(u = 0; u < n; ++u) printf("used[%d] == %d ", u, arc[1 + n * n + u]);
			printf("\n");
			j += dfs_in[2 + (n * n) + n];
		}
	}
	arc[2 + (n * n) + n] = j;
	printf("we in %d , j == %d \n",v, arc[2 + (n * n) + n] = j);
	return 0;
}

int main(int argc, char* argv[]){
	FILE *fd_1 = fopen(argv[1], "r");
	if(fd_1 == NULL){
		perror("failed to open file for reading");
		exit(1);
	}
	int n = 0;
	fscanf(fd_1, "%d", &n);
	int m = 0;
	fscanf(fd_1, "%d", &m);
	int **graf = (int **)malloc(m*sizeof(int*));
	for(int i = 0; i < m; ++i){
		graf[i] = (int *)malloc(2*sizeof(int));
	}
	for(int i = 0; i < m; ++i){
		fscanf(fd_1, "%d", &graf[i][0]);
		fscanf(fd_1, "%d", &graf[i][1]);
	}
	fclose(fd_1);
	int **grap = (int **)malloc(n*sizeof(int*));
        for(int i = 0; i < n; ++i){
                grap[i] = (int *)malloc(n*sizeof(int));
        }
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			grap[i][j] = -1;
		}
	}
	for(int i = 0; i < m; ++i){
		for(int j = 0; j < n; ++j){
			if(grap[graf[i][0]][j] == -1){
				grap[graf[i][0]][graf[i][1]] = graf[i][1];
			}
		}
	}
	for(int i = 0; i < n; ++i){
                for(int j = 0; j < n; ++j){
                        printf("%d ", grap[i][j]);
                }
		printf("\n");
        }
	printf("\n");
	int* used = (int *)malloc(n*sizeof(int));
	for(int i = 0; i < n; ++i){
		used[i] = 0;
	}
	int total = 1;
	int* dfs_in = (int *)malloc((3 + (n * n) + n)*sizeof(int));
	dfs_in[0] = n;
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			dfs_in[1 + n * i + j] = grap[i][j];
		}
	}
	for(int i = 0; i < n; ++i){
		dfs_in[1 + n * n + i] = used[i];
	}
	dfs_in[1 + n * n + n] = 0;
	for(int i = 0; i < 2 + (n*n) +n; ++i) printf("%d ", dfs_in[i]);
	printf("\n");
	for(int i = 0; i < n; ++i){
		if(dfs_in[1 + n*n + i] == 0){
			int s_err_thread = 0;
                        pthread_t thread;
			s_err_thread = pthread_create(&thread, NULL, dfs, dfs_in);
                        if(s_err_thread != 0){
                                perror("error in thread create in main");
                                exit(1);
                        }
                        pthread_join(thread, NULL);
			total += dfs_in[2 + (n * n) + n];
		}
	}
	printf("total: %d \n", total);
	return 0;
}
