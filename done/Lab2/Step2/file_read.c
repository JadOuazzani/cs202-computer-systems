#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// max display size for names
#define NAME_DISPLAY_SIZE 15
#define NAME_MAX_SIZE 1024


// A bit of tricks
#define SSTR(X) #X
#define STR(X) SSTR(X)

// ---- Step 2.1.1: wrappers ----
typedef int myFile;

myFile my_fopen(const char* filename, const char* mode) {
    if (strcmp(mode, "r") == 0) {
        return open(filename, O_RDONLY);
    }
    return -1; 
}

int my_fclose(myFile f) {
    return close(f);
}

int my_read(myFile input, char* name, unsigned int* p_age) {
    size_t BUFFER_SIZE = NAME_MAX_SIZE + 6;
    char buffer[BUFFER_SIZE];
    
    off_t offset = lseek(input, 0, SEEK_CUR);
    ssize_t nb_lu = read(input, buffer, BUFFER_SIZE - 1);
    if (nb_lu == -1) return -1 ; 
    buffer[nb_lu] = '\0';
    
    char* end_lign = memchr(buffer, '\n', nb_lu);
    if (end_lign == NULL) return -1; 

    int size_lign =  end_lign - buffer + 1;

    lseek(input, offset + size_lign, SEEK_SET);
    return sscanf(buffer, "%1023s %u", name, p_age) ;
     
}
// -------------------------------

int main(void)
{
    const char* filename = "data.dat"; // hard-coded filename

    // Open file
    myFile input = my_fopen(filename, "r");
    if (input == -1) {
        fprintf(stderr, "Error: cannot open file \"%s\" in read mode!\n", filename);
        return 1;
    }
    printf("File descriptor : %d \n", input);

    // Frame top line
    printf("+-----------------+-----+\n");

    // some stats
    unsigned int nb = 0;
    unsigned int age_max = 0;
    unsigned int age_min = (unsigned int) -1; // trick: -1 is the highest possible unsigned
    double total = 0.0; // notice 'double', to avoid integer division

    // loop on name+age reading from file
    while (1) {
        char name[NAME_DISPLAY_SIZE+1] = "";
        unsigned int age = 0;
        if (my_read(input, name, &age) != 2) {
            break;
        }
        ++nb;             // one more record
        total += age;     // for later average
        // updates stats
        if (age_min > age) age_min = age;
        if (age_max < age) age_max = age;
        // prints: - sign is for left alignment
        printf("| %-" STR(NAME_DISPLAY_SIZE) "s | %3d |\n", name, age);
        
        puts("Press enter to continue");
        getchar(); 
    } 

    my_fclose(input); // don't forget to close file

    // some stats at the end
    printf("+-----------------+-----+\n");
    printf(" âge minimum : %3d\n", age_min);
    printf(" âge maximal : %3d\n", age_max);
    printf("%d people, average age: %5.1f years\n", nb, total / nb);
    return 0;
}