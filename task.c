#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>

char* root;
char* path;
int size = 0;

struct stack{
	struct stack* next;
	char* data;
} *head;

ssize_t read_(int fd, void* buf, size_t count) {
    int res = 0;
    int rres = 0;
    while(1) {
        res = read(fd, buf, count);
        if(res == count) {
            return rres + res;
        }
        if(res == -1) {
            return -1;
        }
        if(res == 0) {
            return rres;
        }
        rres += res;
        buf += res;
        count -= res;
    }
}

ssize_t write_(int fd, void* buf, size_t count) {
    int res = 0;
    int rres = 0;
    while(1) {
        res = write(fd, buf, count);
        if(res == count) {
            return rres + res;
        }
        if(res == -1) {
            return -1;
        }
        rres += res;
        buf += res;
        count -= res;
    }
}

void push(char* ch){
	struct stack *buf = malloc(sizeof(struct stack));
	buf->data = ch;
	if(size == 0)
		head->next = buf;
	else {
		struct stack *buf2 = head->next;
		head->next = buf;
		buf->next = buf2;
	}
	size++;
}

char* pop(){
	if(size == 0)
		return NULL;
	char *data = head->next->data;
	if(size == 1)
		head->next = NULL;
	else
		head->next = head->next->next;
	size--;
	return data;
}

int find(const char* st, char ch){
	int index = -1;
	for(int i = 0; i < strlen(st); i++)
		if(st[i] == ch)
			index = i;
	return index;
}

char *path_to_file(char *file, char* root_dir, char* to_dir){
	size_t size = strlen(file) - strlen(root_dir) + strlen(to_dir) + 1;
	char *to_file = malloc(size * sizeof(char));
	memmove(to_file, to_dir, strlen(to_dir) * sizeof(char));
	memmove(to_file + strlen(to_dir), file + strlen(root_dir), (strlen(file) - strlen(root_dir)) * sizeof(char));
	to_file[size - 1] = 0;
	return to_file;
}

int findAll(int *arr,char *str, char ch){
	int count = 0;
	for(int i = 0; i < strlen(str); i++)
		if(str[i] == ch)
			arr[count++] = i;
	if(count == 0 || count == 1)
		return -1;
	return count;
}

int makedirs(char *file, char *prev_file){
	int* arr = malloc(100 * sizeof(int));;
	int res = findAll(arr, file, '/');
	if(res == -1)
		return 0;
	for(int i = 0; i < res; i++){
		char *buf = malloc(strlen(file) * sizeof(char));
		memmove(buf, file, (arr[i] + 1)*sizeof(char));
		memset(buf+(arr[i] + 1)*sizeof(char), 0, (strlen(buf) - arr[i])*sizeof(char));
		struct stat *st = malloc(sizeof(struct stat));
		printf("%s\n", buf);
		if(stat(buf, st) == -1)
			if(mkdir(buf, 0777) == -1){
				return -1;
			}
	}
	return 0;
}

void report_error() {
    char* err = strerror(errno);
    write_(STDERR_FILENO, err, strlen(err) * sizeof(char));
}

int copy_(int fb_read, int fb_write){
	char buf[4096];
    ssize_t rres = 0;
    ssize_t wres = 0;
    while(1) {
        rres = read_(fb_read, buf, sizeof(buf));
        if(rres == -1) {
            report_error();
            return 1;
        }
        wres = write_(fb_write, buf, rres);
        if(wres == -1) {
            report_error();
            return 1;
        }   
        if(rres < sizeof(buf)) {
            return 0;
        }
    }
}

int copy_content(char * file, char* to_file){
	int fb_in = open(file, O_RDONLY);
	if(fb_in == -1){
		perror("error");
		exit(1);
	}
	printf("makedir for file = %s\n", to_file);
	if(makedirs(to_file, file) == -1){
		printf("test\n");
	}
	printf("next\n");
	struct stat *st = malloc(sizeof(struct stat));
	if(fstat(fb_in, st) == -1){
		perror("exit(1)");
		exit(1);
	}
	int fb_out = open(to_file, O_WRONLY | O_CREAT, st->st_mode);
	if(copy_(fb_in, fb_out) == 1){
		exit(1);
	}
	close(fb_in);
	close(fb_out);
	return 0;
}

int copy_files(char* dir_name, char* to_dir_name){
	DIR * dir;
	struct dirent * entry;
	if((dir = opendir(to_dir_name)) == NULL)
		return -1;
	while((entry = readdir(dir)) != NULL){
		printf("%s %i\n", entry->d_name, entry->d_type);
		if(entry->d_type == 4 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0){
			printf("%s to %s\n", to_dir_name, entry->d_name);
			char *ch_buf = malloc(1024 * sizeof(char));
			ch_buf = strcpy(ch_buf, to_dir_name);
			if(ch_buf[strlen(ch_buf) - 1] != '/')
				strcat(ch_buf, "/");
			printf("%s\n", strcat(ch_buf, entry->d_name));
			if(copy_files(dir_name, ch_buf) == -1)
				printf("error");
		} else if(entry->d_type == 8){
			char *path_file_name = malloc(1024 * sizeof(char));
			path_file_name = strcpy(path_file_name, to_dir_name);
			if(path_file_name[strlen(path_file_name) - 1] != '/')
				strcat(path_file_name, "/");
			push(strcat(path_file_name, entry->d_name));
		}
	}
	printf("close dir %s\n", to_dir_name);
	closedir(dir);
	return 0;
}

int main(int argc, const char** args){
	if(argc != 3){
		root = (char *)malloc(MAXPATHLEN * sizeof(char));
		char *res = realpath("./", root);
		printf("root is %s", root);
		return 0;
	}
	DIR *dir;
	head = malloc(sizeof(struct stack));
	path = malloc(sizeof(char) * 2048 );
	
	char *dir_name = malloc(strlen(args[1]) * sizeof(char));
	strcpy(dir_name, args[1]);
	char *to = malloc(strlen(args[2]) * sizeof(char));
	strcpy(to, args[2]);
	printf("to_dir = %s\n", to);
	root = (char *)malloc(MAXPATHLEN * sizeof(char));
	if(dir_name[strlen(dir_name) - 1] != '/')
		strcat(dir_name, "/");
	root = strcpy(root, dir_name);
	printf("dir_name = %s\n", dir_name);
	copy_files(dir_name, dir_name);
	printf("to = %s\n", to);
	if(to[strlen(to) - 1] != '/')
		strcat(to, "/");
	char *ch;
	int count = 0;
	while((ch = pop()) != NULL){
		char *file = path_to_file(ch, dir_name, to);
		printf("%d, %s vs %s, size = %d\n", ++count, file, ch, size);
		copy_content(ch, file);
	}
	return 0;
}
