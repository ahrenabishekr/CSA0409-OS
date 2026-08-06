#include <stdio.h>
#include <semaphore.h>

sem_t wrt;

void reader() {
    printf("Reader is Reading\n");
}

void writer() {
    sem_wait(&wrt);
    printf("Writer is Writing\n");
    sem_post(&wrt);
}

int main() {
    sem_init(&wrt, 0, 1);

    reader();
    writer();

    sem_destroy(&wrt);

    return 0;
}