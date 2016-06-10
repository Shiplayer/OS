#include <stdio.h>
#include <pthread.h>

#define attr_init(a) {if(pthread_attr_init(&a) != 0) {perror("attr_init"); exit(1);}}

static void *show(void *arg){
    pthread_t pt;
    pthread_attr_t attr;
    attr_init(attr);
    printf("show\n");
    return NULL;
    //exit(0);
}


int main(){
    pthread_t pt;
    pthread_attr_t attr;
    attr_init(attr);
    if(pthread_create(&pt, NULL, &show, NULL) != 0){
        perror("error");
        exit(1);
    }
    if(pthread_join(&pt, 0) != 0){
        perror("join error");
        exit(1);
    }
    return 0;
}
