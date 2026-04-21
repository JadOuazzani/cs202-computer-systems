#include "array_std.h"
#include <stdio.h>

void print_array(int * t, int len) {
    for (int i = 0; i < len; ++i)
        printf("%d ", t[i]);
    printf("\n");
}

void print_before_and_after(int * before, int * after, int len) {
    printf("Before: ");
    print_array(before, len);
    printf("After: ");
    print_array(after, len);
}

void copy_array(int * from, int * to, int len) {
    for (int i = 0; i < len; ++i)
        to[i] = from[i];
}

