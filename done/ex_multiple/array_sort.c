#include "swap.h"
#include "array_filter.h"

void sort_ascending(int * t, int len) {
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (t[i] > t[j]) swap(t + i, t + j);
        }
    }
}

void sort_descending(int * t, int len) {
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (t[i] < t[j]) swap(t + i, t + j);
        }
    }
}

void selection_sort_descending(int * t, int len) {
    int max_index;
    for (int i = 0; i <= len-2; i++){
        max_index =  max_element_index(t + i + 1, len - i - 1) + i + 1;
        if ( t[max_index] > t[i]) swap(t + max_index, t + i);
    }
}

