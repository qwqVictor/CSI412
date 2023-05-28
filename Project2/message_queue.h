#ifndef VICTOR_MESSAGE_QUEUE
#include <pthread.h>

#define VICTOR_MESSAGE_QUEUE

/* Maximum buffer size for messages */
#define MQ_BUFFER_SIZE (100)

/* Queue node structure */
struct message_queue_node {
    char* buffer;                    /* Pointer to message buffer */
    int bufsize;                     /* Size of message buffer */
    struct message_queue_node* next; /* Pointer to next node */
};

/* Queue structure */
struct message_queue {
    pthread_mutex_t mutex;           /* Mutex for thread safety */
    struct message_queue_node* head; /* Pointer to head of queue */
    struct message_queue_node* tail; /* Pointer to tail of queue */
};

/* Function prototypes */
extern int mq_initialize(struct message_queue* q);
extern int mq_push(struct message_queue* q, char const* from, int size);
extern int mq_pop(struct message_queue* q, char* dest, int size);
extern int mq_destroy(struct message_queue* q);

#endif /* End of VICTOR_MESSAGE_QUEUE */