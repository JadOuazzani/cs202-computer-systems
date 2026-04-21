#include <stdio.h>
#include <string.h>
// max display size for names
#define NAME_DISPLAY_SIZE 15
// A bit of tricks
#define SSTR(X) #X
#define STR(X) SSTR(X)

int main(void)
{
    const char* filename = "data.dat"; // hard-coded filename
    // Open file
    FILE* input = fopen(filename, "r");
    if (input == NULL) {
        fprintf(stderr, "Error: cannot open file \"%s\" in read mode!\n", filename);
    return 1;
    }   
    // Frame top line
    printf("+-----------------+-----+\n");
    // some stats
    unsigned int nb = 0;
    unsigned int age_max = 0;
    unsigned int age_min = (unsigned int) -1; // trick: -1 is the highest possible unsigned
    double total = 0.0; // notice'double', to avoid integer division
    // loop on name+age reading from file
    do {
        char name[NAME_DISPLAY_SIZE+1] = "";
        unsigned int age = 0;
            if (fscanf(input,"%15s %u", name, &age) == 2) {
                ++nb; // one more record
                total += age; // for later average
                // updates stats
                if (age_min > age) age_min = age;
                if (age_max < age) age_max = age;
                // prints: - sign is for left alignment
                printf("| %-" STR(NAME_DISPLAY_SIZE) "s | %3d |\n", name, age);
            }
    } while (! feof(input));
    fclose(input); // don't forget to close file
    // some stats at the end
    printf("+-----------------+-----+\n");
    printf(" âge minimum : %3d\n", age_min);
    printf(" âge maximal : %3d\n", age_max);
    printf("%d people, average age: %5.1f years\n", nb, total / nb);
    return 0;
}  