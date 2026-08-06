#include <stdio.h>

int main() {
    int available = 3;
    int max = 7, allocated = 4;

    int need = max - allocated;

    if(need <= available)
        printf("Safe State\n");
    else
        printf("Unsafe State\n");

    return 0;
}