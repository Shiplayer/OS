#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test(){
	printf("exit(0)\n");
	exit(0);
}

int main(int argc, char **argv[]){
	/*int pipefb[2];
	pipe(pipefb);
	char *buf = malloc(sizeof(char) * 1024);
	pid_t pid = fork();
	if(pid == 0){
		close(pipefb[0]);
		read(STDIN_FILENO, buf, sizeof(char) * 1024);
		write(pipefb[1], buf, strlen(buf));
		close(pipefb[1]);
		_exit(EXIT_SUCCESS);
	} else{
		close(pipefb[1]);
//		wait(NULL);
		read(pipefb[0], buf, sizeof(char) * 1024);
		write(STDOUT_FILENO, buf, strlen(buf));
		close(pipefb[0]);
		exit(EXIT_SUCCESS); 
	}*/
	test();
	printf("test");
	return;

}
