#include "macros.h"
#include "message_queue.h"
#include <semaphore.h>
#include <string.h>
#include <sys/shm.h>

// declaration of functions
void* readerFunc(void* args);
void* characterFunc(void* args);
void* toUpperFunc(void* args);
void* writerFunc(void* args);

// semaphore and message queue variables
sem_t semReaderToCharacter, semCharacterToToUpper, semToUpperToWriter;
struct message_queue readerToCharacter, characterToToUpper, toUpperToWriter;

int main(int argc, char const* argv[]) {
    pthread_t reader, character, toUpper, writer;

    // check if enough arguments are provided
    if (argc < 4) {
        fprintf(stderr, "Not enough arguments.\n");
        return 1;
    }

    // initialize semaphores
    _try(sem_init(&semReaderToCharacter, 0, 0));
    _try(sem_init(&semCharacterToToUpper, 0, 0));
    _try(sem_init(&semToUpperToWriter, 0, 0));

    // create threads for each function
    _try(pthread_create(&reader, NULL, readerFunc, (void*)argv[2]));
    _try(pthread_create(&character, NULL, characterFunc, (void*)argv[3]));
    _try(pthread_create(&toUpper, NULL, toUpperFunc, (void*)NULL));
    _try(pthread_create(&writer, NULL, writerFunc, (void*)argv[1]));

    // join threads
    pthread_join(reader, NULL);
    pthread_join(character, NULL);
    pthread_join(toUpper, NULL);
    pthread_join(writer, NULL);

    // destroy semaphores
    _try(sem_destroy(&semReaderToCharacter));
    _try(sem_destroy(&semCharacterToToUpper));
    _try(sem_destroy(&semToUpperToWriter));
    return 0;
}

void* readerFunc(void* args) {
    // initialize message queue
    mq_initialize(&readerToCharacter);
    char buf[MQ_BUFFER_SIZE];
    char* filename = args;
    FILE* f = fopen(filename, "r");

    // read from file
    if (f != NULL) {
        while (!feof(f)) {
            fgets(buf, MQ_BUFFER_SIZE, f);
            int len = strlen(buf);
            debug("read buffer from line %s", buf);
            _try_do(mq_push(&readerToCharacter, buf, len + 1), sem_post(&semReaderToCharacter));
            bzero(buf, MQ_BUFFER_SIZE);
        }
        sem_post(&semReaderToCharacter);
        fclose(f);
    }
    pthread_exit(NULL);
}

void* characterFunc(void* args) {
    // initialize message queue
    mq_initialize(&characterToToUpper);
    char buf[MQ_BUFFER_SIZE];
    char ch = ((char*)args)[0];

    // loop forever and read from message queue
    while (1) {
        sem_wait(&semReaderToCharacter);
        int ret = mq_pop(&readerToCharacter, buf, MQ_BUFFER_SIZE);

        // destroy message queue and exit from loop
        if (ret < 0) {
            mq_destroy(&readerToCharacter);
            sem_post(&semCharacterToToUpper);
            break;
        }

        // perform character transformation
        int len = strlen(buf);
        for (int i = 0; i < len; i++) {
            switch (buf[i]) {
            case ' ':
            case '\n':
            case '\t':
            case '\r':
                buf[i] = ch;
                break;
            default:
                break;
            }
        }
        _try_do(mq_push(&characterToToUpper, buf, len + 1), sem_post(&semCharacterToToUpper));
    }
    pthread_exit(NULL);
}

void* toUpperFunc(void* args) {
    // initialize message queue
    mq_initialize(&toUpperToWriter);
    char buf[MQ_BUFFER_SIZE];

    while (1) {
        sem_wait(&semCharacterToToUpper);
        int ret = mq_pop(&characterToToUpper, buf, MQ_BUFFER_SIZE);

        // destroy message queue and exit from loop
        if (ret < 0) {
            mq_destroy(&characterToToUpper);
            sem_post(&semToUpperToWriter);
            break;
        }

        // perform uppercase transformation
        int len = strlen(buf);
        for (int i = 0; i < len; i++) {
            if ('a' <= buf[i] && buf[i] <= 'z') {
                buf[i] = buf[i] - 'a' + 'A';
            }
        }
        _try_do(mq_push(&toUpperToWriter, buf, len + 1), sem_post(&semToUpperToWriter));
    }
    pthread_exit(NULL);
}

void* writerFunc(void* args) {
    // initialize variables
    char buf[MQ_BUFFER_SIZE];
    char path[255];
    char template[] = "/tmp/proj2.XXXXXX";
    mkdtemp(template);
    sprintf(path, "%s/%s", template, (char*)args);
    FILE* f = fopen(path, "w");

    // loop forever and read from message queue
    if (f != NULL) {
        while (1) {
            sem_wait(&semToUpperToWriter);
            int ret = mq_pop(&toUpperToWriter, buf, MQ_BUFFER_SIZE);
            debug("got ret %d buffer %s", ret, buf);

            // exit from loop
            if (ret < 0) {
                break;
            }

            // write to file
            fputs(buf, f);
            fputc('\n', f);
            bzero(buf, MQ_BUFFER_SIZE);
        }
        fclose(f);
    }
    mq_destroy(&toUpperToWriter);
    printf("%s\n", path);
    pthread_exit(NULL);
}
