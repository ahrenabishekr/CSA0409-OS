#include <stdio.h>
#include <pthread.h>

int count = 0;
pthread_mutex_t lock;

void *increment(void *arg) {
    pthread_mutex_lock(&lock);
    count++;
    printf("Count = %d\n", count);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t t1;

    pthread_mutex_init(&lock, NULL);
    pthread_create(&t1, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}