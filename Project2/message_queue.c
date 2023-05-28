#include "message_queue.h"

/* Function to initialize a message queue struct */
int mq_initialize(struct message_queue* q) {
    if (q == NULL) // Check if message queue struct is NULL
        return -1;
    q->head = NULL; // Initialize head and tail nodes to NULL
    q->tail = NULL;
    return pthread_mutex_init(&q->mutex, NULL); // Initialize mutex
}

/* Function to push a message onto the message queue */
int mq_push(struct message_queue* q, char const* from, int size) {
    int ret = 0;
    if (size > MQ_BUFFER_SIZE)
        return -1; // Check if buffer size is greater than MQ_BUFFER_SIZE
    int pthread_ret;
    if (pthread_ret = pthread_mutex_lock(&q->mutex)) {
        return pthread_ret; // Check if mutex lock returns any errors
    }
    /* Allocate memory for a new node and copy the message to the node's buffer */
    struct message_queue_node* node = malloc(sizeof(struct message_queue_node));
    node->buffer = malloc(size);
    node->bufsize = size;
    node->next = NULL;
    memcpy(node->buffer, from, size);
    /* Update the head and tail nodes of the message queue */
    if (q->tail != NULL)
        q->tail->next = node;
    if (q->head == NULL)
        q->head = node;
    q->tail = node;
    if (pthread_ret = pthread_mutex_unlock(&q->mutex)) {
        return pthread_ret; // Check if mutex unlock returns any errors
    }
    return ret;
}

/* Function to pop a message from the message queue */
int mq_pop(struct message_queue* q, char* dest, int size) {
    int ret = 0;
    int pthread_ret;
    struct message_queue_node* node = NULL;
    if (pthread_ret = pthread_mutex_lock(&q->mutex)) {
        return pthread_ret; // Check if mutex lock returns any errors
    }
    /* Check if message queue is empty or if message buffer size is less than required size */
    if (q->head == NULL) {
        ret = -2;
        goto MQ_POP_PTHREAD_DEFER;
    }
    if (size < q->head->bufsize) {
        ret = -1;
        goto MQ_POP_PTHREAD_DEFER;
    }
    /* Update head and tail nodes of the message queue and free the popped node's buffer */
    node = q->head;
    q->head = node->next;
    if (q->head == NULL)
        q->tail = NULL;
MQ_POP_PTHREAD_DEFER:
    if (pthread_ret = pthread_mutex_unlock(&q->mutex)) {
        return pthread_ret; // Check if mutex unlock returns any errors
    }
    /* Copy the popped message to the destination buffer and free the popped node */
    if (node != NULL) {
        memcpy(dest, node->buffer, (node->bufsize < size) ? node->bufsize : size);
        free(node);
    }
    return ret;
}

/* Function to destroy a message queue struct and free all memory */
int mq_destroy(struct message_queue* q) {
    if (q == NULL)
        return -1;
    int pthread_ret;
    if (pthread_ret = pthread_mutex_lock(&q->mutex)) {
        return pthread_ret; // Check if mutex lock returns any errors
    }
    /* Free all memory used by the message queue nodes and destroy the mutex */
    for (struct message_queue_node* node = q->head; node != NULL;) {
        free(node->buffer);
        struct message_queue_node* prev = node;
        node = node->next;
        free(prev);
    }
    q->head = NULL; // Set head and tail nodes to NULL
    q->tail = NULL;
    if (pthread_ret = pthread_mutex_unlock(&q->mutex)) {
        return pthread_ret; // Check if mutex unlock returns any errors
    }
    if (pthread_ret = pthread_mutex_destroy(&q->mutex)) {
        return pthread_ret; // Check if mutex destroy returns any errors
    }
}