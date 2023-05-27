#include "message_queue.h"
#include <stdlib.h>
#include <string.h>

int mq_initialize(struct message_queue* q) {
    if (q == NULL)
        return -1;
    q->head = NULL;
    q->tail = NULL;
    return pthread_mutex_init(&q->mutex, NULL);
}

int mq_push(struct message_queue* q, char const* from, int size) {
    int ret = 0;
    if (size > MQ_BUFFER_SIZE) return -1;
    int pthread_ret;
    if (pthread_ret = pthread_mutex_lock(&q->mutex)) {
        return pthread_ret;
    }
    struct message_queue_node* node = malloc(sizeof(struct message_queue_node));
    node->buffer = malloc(size);
    node->bufsize = size;
    node->next = NULL;
    memcpy(node->buffer, from, size);
    if (q->tail != NULL)
        q->tail->next = node;
    if (q->head == NULL)
        q->head = node;
    q->tail = node;
    if (pthread_ret = pthread_mutex_unlock(&q->mutex)) {
        return pthread_ret;
    }
    return ret;
}

int mq_pop(struct message_queue* q, char* dest, int size) {
    int ret = 0;
    int pthread_ret;
    struct message_queue_node* node = NULL;
    if (pthread_ret = pthread_mutex_lock(&q->mutex)) {
        return pthread_ret;
    }
    if (q->head == NULL) {
        ret = -2;
        goto MQ_POP_PTHREAD_DEFER;
    }
    if (size < q->head->bufsize) {
        ret = -1;
        goto MQ_POP_PTHREAD_DEFER;
    }
    node = q->head;
    q->head = node->next;
    if (q->head == NULL)
        q->tail = NULL;
    MQ_POP_PTHREAD_DEFER:
    if (pthread_ret = pthread_mutex_unlock(&q->mutex)) {
        return pthread_ret;
    }
    if (node != NULL) {
        memcpy(dest, node->buffer, (node->bufsize < size) ? node->bufsize : size);
        free(node);
    }
    return ret;
}

int mq_destroy(struct message_queue* q) {
    if (q == NULL)
        return -1;
    int pthread_ret;
    if (pthread_ret = pthread_mutex_lock(&q->mutex)) {
        return pthread_ret;
    }
    for (struct message_queue_node* node = q->head; node != NULL;) {
        free(node->buffer);
        struct message_queue_node* prev = node;
        node = node->next;
        free(prev);
    }
    q->head = NULL;
    q->tail = NULL;
    if (pthread_ret = pthread_mutex_unlock(&q->mutex)) {
        return pthread_ret;
    }
    if (pthread_ret = pthread_mutex_destroy(&q->mutex)) {
        return pthread_ret;
    }
}