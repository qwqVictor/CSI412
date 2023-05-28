#ifndef VICTOR_MESSAGE_QUEUE
#include <pthread.h>
#define VICTOR_MESSAGE_QUEUE

#define MQ_BUFFER_SIZE (100)

struct message_queue_node {
    char* buffer;
    int bufsize;
    struct message_queue_node* next;
};

struct message_queue {
    pthread_mutex_t mutex;
    struct message_queue_node* head;
    struct message_queue_node* tail;
};

extern int mq_initialize(struct message_queue* q);
extern int mq_push(struct message_queue* q, char const* from, int size);
extern int mq_pop(struct message_queue* q, char* dest, int size);
extern int mq_destroy(struct message_queue* q);

#endif