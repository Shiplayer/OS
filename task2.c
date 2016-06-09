#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>

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

void report_error() {
    char* err = strerror(errno);
    write_(STDERR_FILENO, err, strlen(err) * sizeof(char));
}

int copy_(int fb_read, int fb_write){
	char buf[4096];
    ssize_t rres = 0;
    ssize_t wres = 0;
    while(1) {
        rres = read_(fb_read, buf, 4096 * sizeof(char));
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

int createPath(char* path){
    int status;
    struct stat st;
    printf("start creating dir\n");
    if(stat(path, &st) != 0){
        char *updir;
        printf("start creating dir\n");
        for(int i = 0; i < strlen(path); i++){
            if(path[i] == '/'){
                updir = malloc(i * sizeof(char));
                printf("copy(%s, %d)\n", path, i);
                strncpy(updir, path, i * sizeof(char));
                printf("%s\n", updir);
                if(stat(updir, &st) != 0){
                    if(mkdir(updir, 0777) == 0){
                        printf("created dir(%s)\n", updir);
                    } else
                        return -1;
                }
                printf("dir is created\n");
            }
        }
    }
    printf("methods createPath is finished\n");
    return 0;
}

int checkToPath(char *toPath){
    int length = strlen(toPath);
    if(length == 1){
        return 0;
    }
    printf("%s\n", toPath);
    for(int i = length - 1; i >= 0; i--){
        if(toPath[i] == '/'){
            printf("createPath\n");
            if(createPath(toPath) == -1){
                perror("can't create dir");
                exit(1);
            }
            break;
        }
    }
    printf("method checkToPath is finished\n");
    return 1;
}

char *getFileName(char *path){
    for(int i = strlen(path) - 1; i >= 0; i--){
        if(path[i] == '/'){
            char *buf = malloc((strlen(path) - i) * sizeof(char));
            strcpy(buf, path + i + 1);
            return buf;
        }
    }
    return path;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("require 3 args");
        return 0;
    }
    int status;
    struct stat st;
    char *from = malloc(strlen(argv[1]) * sizeof(char));
    strcpy(from, argv[1]);
    char *to = malloc(strlen(argv[2]) * sizeof(char));
    strcpy(to, argv[2]);
    status = stat(from, &st);
    if(status != 0){
        perror("stat returned error");
        exit(1);
    }
    if(S_ISREG(st.st_mode)){
        printf("is file\n");
        //if(checkToPath(argv[2]) == 0){
        checkToPath(to);
        int len = strlen(to) - 1;
        if(to[len] == '/'){
            char *fileName = getFileName(from);
            char *path;
            printf("fileName = %s\n", fileName);
            path = malloc((strlen(to) + strlen(fileName) + 1) * sizeof(char));
            strcpy(path, to);
            for(int i = strlen(path), j = 0; i < strlen(path) + strlen(fileName); i++, j++){
                path[i] = fileName[j];
            }
            path[strlen(to) + strlen(fileName)] = '\0';
            printf("copy_content(%s, %s)\n", from, path);
            copy_content(from, path);
        } else {
            copy_content(from, to);
        }

    }
    if(S_ISDIR(st.st_mode)){

    }
    return 0;
}
