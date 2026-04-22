#include <stdio.h>
#include <unistd.h>

int main(void){
    const char* filename = "data.dat";

    printf("Press enter to delete file \"%s\"\n", filename);
    getchar();

    int result = unlink(filename); 

    if (result == 0) {
        printf("File \"%s\" deleted successfully.\n", filename);
    } else {
        perror("Failed to delete file");
        return 1;
    }
    return 0;
    
}