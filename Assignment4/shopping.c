/* shopping.c */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

// Declare semaphores
sem_t sem_produce;
sem_t sem_dairy;

void* print_produce(void*);
void* print_dairy(void*);

void* print_dairy(void* items) {
    int i = 0;
    char** array = (void*)items;
    {
        // Wait for produce to print Salad
        sem_wait(&sem_dairy);
        printf("got %s\n", (array[i++]));
        // Signal produce to print Butter
        sem_post(&sem_produce);
        printf("got %s\n", (array[i++]));
    }
    return (NULL);
}

void* print_produce(void* items) {
    int i = 0;
    char** array = (void*)items;
    {
        printf("got %s\n", (array[i++]));
        // Signal dairy to print Milk
        sem_post(&sem_dairy);
        // Wait for dairy to print Butter
        sem_wait(&sem_produce);
        printf("got %s\n", (array[i++]));
    }
    return (NULL);
}

int main() {
    char* produce[] = {"Salad", "Apples", NULL};
    char* dairy[] = {"Milk", "Butter", NULL};

    // Initialize semaphores
    sem_init(&sem_produce, 0, 0);
    sem_init(&sem_dairy, 0, 0);

    pthread_t th1, th2;
    pthread_create(&th1, NULL, print_produce, (void*)produce);
    pthread_create(&th2, NULL, print_dairy, (void*)dairy);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    // Destroy semaphores
    sem_destroy(&sem_produce);
    sem_destroy(&sem_dairy);
}