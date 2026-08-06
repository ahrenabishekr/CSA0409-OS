#include <stdio.h>

int mutex = 1, full = 0, empty = 3;

void producer() {
    mutex--;
    empty--;
    full++;
    printf("Produced\n");
    mutex++;
}

void consumer() {
    mutex--;
    full--;
    empty++;
    printf("Consumed\n");
    mutex++;
}

int main() {
    producer();
    consumer();
    return 0;
}