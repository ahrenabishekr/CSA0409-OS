#include <stdio.h>

int main() {
    int block[5] = {100, 500, 200, 300, 600};
    int process[4] = {212, 417, 112, 426};
    int i, j;

    for(i = 0; i < 4; i++) {
        for(j = 0; j < 5; j++) {
            if(block[j] >= process[i]) {
                printf("Process %d allocated to Block %d\n", i + 1, j + 1);
                block[j] -= process[i];
                break;
            }
        }
    }
    return 0;
}