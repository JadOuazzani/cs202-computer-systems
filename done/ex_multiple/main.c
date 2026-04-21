#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array_std.h"
#include "array_sort.h"

int main() {
    //--------------------------------------------------------------
    // Getting array length, min limit, and max limit from the user.

    printf("Enter array length:\n");
    int len;
    scanf("%d", &len);
    if (len <= 0) {
        printf("Error: length must be a positive number. Exiting...\n");
        return 0;
    }

    printf("Enter the min and max allowed array element:\n");
    int range_min, range_max;
    scanf("%d %d", &range_min, &range_max);
    if (range_max < range_min) {
        printf("Error: max should be higher than min. Exiting...\n");
        return 0;
    }
    //--------------------------------------------------------------

    int * t = malloc(len * sizeof(int));
    int * t_copy = malloc(len * sizeof(int));
    memset(t, 0, len * sizeof(int)); // fill in the array with zeros
    memset(t_copy, 0, len * sizeof(int));

    for(int i = 0; i < len; i++)
        t[i] =  range_min + rand() % (range_max - range_min);
    printf("Array has %d elements: \n", len);
    print_array(t, len);

    printf("\n\n============== Sorting in ascending order ==============\n");

    copy_array(t, t_copy, len);
    sort_ascending(t_copy, len);
    print_before_and_after(t, t_copy, len);

    printf("\n\n============== Selection sort (descending)==============\n");
    copy_array(t, t_copy, len);
    selection_sort_descending(t_copy, len);
    print_before_and_after(t, t_copy, len);

    //----------------------------------------------------
    // Always remember to free the allocated memory space!
    free(t);
    free(t_copy);
    //----------------------------------------------------

    return 0;
}
