#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
// max display size for names
#define NAME_DISPLAY_SIZE 15
// A bit of tricks
#define SSTR(X) #X
#define STR(X) SSTR(X)

typedef int myFile;

myFile my_fopen(const char* filename, const char* mode){
    int c;
    if (strcmp(mode, "r") == 0) {
        c = O_RDONLY;
        return open(filename, c );
    }

    else if (strcmp(mode, "w") == 0){
        c = O_WRONLY | O_CREAT | O_TRUNC;
        return open(filename, c, 0644);
    }
    
    return -1;
} 

int my_fclose(myFile file) {
    return close(file);
}

int my_read(myFile input, char* name, unsigned int* p_age) {
    char buffer[1030];
    off_t start_pos = lseek(input, 0, SEEK_CUR);
    int bytes_read = read(input, buffer, sizeof(buffer) -1);

    if (bytes_read <= 0) {
        return 0;
    }

    buffer[bytes_read] = '\0';
    char* newline_ptr = strchr(buffer, '\n');
    if (newline_ptr != NULL) {
        int line_length = (newline_ptr-buffer) + 1;
        lseek(input, start_pos + line_length, SEEK_SET);
    }

    return sscanf(buffer, "%s %u", name, p_age);


}

int main(void) {
    const char* filename = "data.dat"; // hard-coded filename
    // Open file
    myFile input = my_fopen(filename, "r");
    if (input == -1) {
        fprintf(stderr, "Error: cannot open file \"%s\" in read mode!\n", filename);
        return 1;
    }
    // Frame top line
    printf("+-----------------+-----+\n");
    // some stats
    unsigned int nb = 0;
    unsigned int age_max = 0;
    unsigned int age_min = (unsigned int) -1; // trick: -1 is the highest possible unsigned
    double total = 0.0; // notice 'double', to avoid integer division
    // loop on name+age reading from file
// --- NOUVELLE BOUCLE DE LECTURE ---
    char name[NAME_DISPLAY_SIZE+1];
    unsigned int age;

    // Tant que my_read réussit à lire exactement 2 éléments (nom et âge)
    while (my_read(input, name, &age) == 2) {
        ++nb; // un enregistrement de plus
        total += age; // pour la moyenne
        
        // mise à jour des stats
        if (age_min > age) age_min = age;
        if (age_max < age) age_max = age;
        
        // affichage
        printf("| %-" STR(NAME_DISPLAY_SIZE) "s | %3d |\n", name, age);
        printf("Appuie sur Entrée pour lire la ligne suivante...\n");
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
