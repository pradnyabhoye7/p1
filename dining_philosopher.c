// 3. Dining Philosophers Problem

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 

sem_t chopsticks[N];

void *philosopher(void *arg) {
    int id = *(int *)arg;

    while (1) {
        printf("Philosopher %d is thinking...\n", id);
        sleep(rand() % 3);

        sem_wait(&chopsticks[id]);
        sem_wait(&chopsticks[(id + 1) % N]);

        printf("Philosopher %d is eating...\n", id);
        sleep(rand() % 2);

        sem_post(&chopsticks[id]);
        sem_post(&chopsticks[(id + 1) % N]);
    }
    return NULL;
}

int main() {
    pthread_t philosophers[N];
    int id[N];

    for (int i = 0; i < N; i++)
        sem_init(&chopsticks[i], 0, 1);

    for (int i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &id[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(philosophers[i], NULL);

    for (int i = 0; i < N; i++)
        sem_destroy(&chopsticks[i]);

    return 0;
}

