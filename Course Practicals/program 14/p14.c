#include <stdio.h>

int main() {
    char user[20], file[20];

    printf("Enter User Name: ");
    scanf("%s", user);

    printf("Enter File Name: ");
    scanf("%s", file);

    printf("\nDirectory Structure\n");
    printf("%s/%s\n", user, file);

    return 0;
}