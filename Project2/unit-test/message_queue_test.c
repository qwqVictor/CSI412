#include "message_queue.h"
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct message_queue q;

struct argtype {
    int id;
    char buffer[10];
    int bufsize;
    int sleeptime;
};

void* thread_test(void* args) {
    char buf[10];
    struct argtype* arg = args;
    fprintf(stderr, "thread %d: mq_push: %d\n", arg->id, mq_push(&q, arg->buffer, arg->bufsize));
    usleep(1000 * arg->sleeptime);
    fprintf(stderr, "thread %d: mq_pop: %d\n", arg->id, mq_pop(&q, buf, 10));
    puts(buf);
    bzero(buf, 10);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    fprintf(stderr, "mq_initialize: %d\n", mq_initialize(&q));

    pthread_t new1, new2, new3;
    pthread_create(&new1, NULL, thread_test, &(struct argtype){
        1,
        "test1",
        sizeof("test1"),
        1500
    });
    pthread_create(&new2, NULL, thread_test, &(struct argtype){
        2, 
        "test2",
        sizeof("test2"),
        100
    });
    pthread_create(&new3, NULL, thread_test, &(struct argtype){
        3, 
        "test3",
        sizeof("test3"),
        50
    });
    pthread_join(new1, NULL);
    pthread_join(new2, NULL);
    pthread_join(new3, NULL);
    fprintf(stderr, "mq_destroy: %d\n", mq_destroy(&q));
    return 0;
}
