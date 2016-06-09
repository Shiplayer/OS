#include <stdio.h>
#include <pthread.h>

#define report(a) {perror(a); exit(1);}

void show(){
    pthread_t pt;
    pthread_attr_t attr;
    if(pthread_attr_init(&attr) != 0){report("error");}
    printf("show\n");
}


int main(){
    pthread_t pt;
    pthread_attr_t attr;
    if(pthread_attr_init(&attr) != 0){
        perror("error attr");
        exit(1);
    }
    if(pthread_create(&pt, &attr, show, NULL) != 0){
        perror("error");
        exit(1);
    }
    if(pthread_join(&pt, NULL) != 0){
        perror("join error");
        exit(1);
    }
    return 0;
}
