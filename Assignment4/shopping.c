/* shopping.c */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

void* print_produce(void*);
void* print_dairy(void*);

void* print_dairy(void* items) {
    int i = 0;
    char** array = (void*)items;
    {
        printf("got %s\n", (array[i++]));
        printf("got %s\n", (array[i++]));
    }
    return (NULL);
}

void* print_produce(void* items) {
    int i = 0;
    char** array = (void*)items;
    {
        printf("got %s\n", (array[i++]));
        printf("got %s\n", (array[i++]));
    }
    return (NULL);
}

int main() {
    char* produce[] = {"Salad", "Apples", NULL};
    char* dairy[] = {"Milk", "Butter", NULL};
    pthread_t th1, th2;
    pthread_create(&th1, NULL, print_produce, (void*)produce);
    pthread_create(&th2, NULL, print_dairy, (void*)dairy);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
}
