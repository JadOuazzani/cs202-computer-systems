#include <stdio.h>
#include <string.h>
// max size for a name
#define NAME_MAX_SIZE 1024

typedef FILE* myFile;

myFile my_fopen(const char* filename, const char* mode){
    return fopen(filename, mode);
}

int my_fclose(myFile file){
    return fclose(file);
}

void my_write(myFile output, const char* name, unsigned int age){
    fprintf(output,"%s %d\n", name, age); 
}

int main(void){
    const char* filename = "data.dat"; // hard-coded filename
    // Open file
    myFile output = my_fopen(filename, "w");
    if (output == NULL) {
        fprintf(stderr, "Error: cannot open file \"%s\" in write mode!\n", filename);
        return 1;
    }   
    // loop on user input: proceed a long as stdin is open
    while (! feof(stdin)) {
        // variables for user interaction
        char name[NAME_MAX_SIZE+1] = "";
        size_t read_size;
        // repeat until we don't have an empty name
        do {
            printf("Enter a name (CTRL+D to end): "); fflush(stdout);
            fgets(name, NAME_MAX_SIZE+1, stdin);
            read_size = strlen(name);
            if ((read_size > 0) && (name[--read_size] == '\n')) name[read_size] = '\0';
        } while (!feof(stdin) && (read_size < 1));
        
        unsigned int age = 0;
        if (! feof(stdin)) {
            // We indeed got an name and can proceed further for the age
            printf("age: "); 
            fflush(stdout);
            if (scanf("%u", &age) != 1) {
            printf("Looking for an age (i.e., positive integer), not ""anything else.\nThis record is canceled.\n");
            while (getc(stdin) != '\n'); // clearing buffer out
            } else {
                getc(stdin); // gets the residual \n
                // write to file
                my_write(output, name, age);
            }
        }
    }
    // purism: end-of-line to properly end the question
    putchar('\n');
    my_fclose(output); // don't forget to close file
    return 0;
}