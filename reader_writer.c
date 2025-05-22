// 2. Reader-Writer Problem

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex, writeLock;
int readCount = 0;

void *reader(void *arg) {
    int id = *(int *)arg;
    sem_wait(&mutex);
    readCount++;
    if (readCount == 1) 
        sem_wait(&writeLock);
    sem_post(&mutex);

    printf("Reader %d is reading...\n", id);
    sleep(1);

    sem_wait(&mutex);
    readCount--;
    if (readCount == 0)
        sem_post(&writeLock);
    sem_post(&mutex);

    return NULL;
}

void *writer(void *arg) {
    int id = *(int *)arg;
    sem_wait(&writeLock);

    printf("Writer %d is writing...\n", id);
    sleep(1);

    sem_post(&writeLock);
    return NULL;
}

int main() {
    pthread_t r[5], w[2];
    int id[5];

    sem_init(&mutex, 0, 1);
    sem_init(&writeLock, 0, 1);

    for (int i = 0; i < 5; i++) {
        id[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &id[i]);
    }
    
    for (int i = 0; i < 2; i++) {
        id[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &id[i]);
    }

    for (int i = 0; i < 5; i++)
        pthread_join(r[i], NULL);
    
    for (int i = 0; i < 2; i++)
        pthread_join(w[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&writeLock);

    return 0;
}
