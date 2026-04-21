#include <stdio.h>

int main() {
    printf("Enter n: ");
    int n = -1;
    scanf("%d", &n);

    if (n < 0) {
        printf("Invalid input value!\n");
        return 0;
    }

    int sum = 0;
    const int bound = 2*n;
    for (int i = 1; i < bound; i += 2) {
        sum += i;
    }
    printf("Sum of the first %d odd numbers = %d\n", n, sum);

    return 0;
}
