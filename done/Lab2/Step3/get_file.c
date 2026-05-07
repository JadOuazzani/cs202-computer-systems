#include <stdio.h>
#include "socket_layer.h"
#include "error.h"
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_FILE_NAME 1040

int main (void){
    int passive_socket = tcp_server_init(CS202_DEFAULT_IP, CS202_DEFAULT_PORT); 
    if (passive_socket < 0 ) {
        perror("server init failed");
        return 1;
    }
    printf("Server listening on %s:%u\n", CS202_DEFAULT_IP, CS202_DEFAULT_PORT);

    while (1) {
        struct sockaddr_in addr; 
        int active_socket = tcp_accept(passive_socket, &addr);
        if (active_socket < 0){
            perror("accept failed");
            return 1;
        }
        char filename[MAX_FILE_NAME] = {0};
        int i = 0; 
        char c0; 
        while (tcp_read(active_socket, &c0, 1) > 0){
            filename[i++] = c0;
            if (c0 == '\0') {
                break;
            }
        }
        printf("%s\n",filename); 
        char* actual_filename = filename + strlen("FILE: ");

        char buf1[64] = {0};
        int j = 0;
        char c1;  
        while (tcp_read(active_socket, &c1, 1) > 0){
            if (c1 == '|') break ;
            buf1[j++] = c1; 
        }
        buf1[j] = '\0'; 
        printf("FileSize is: %s\n", buf1);

        

        long filesize; 
        sscanf(buf1, "%ld", &filesize); 
        
        FILE* output = fopen(actual_filename, "wb"); 
        if (output == NULL){
            perror("cannot create file");
            close(active_socket);
            continue; 
        }
        
        char buf2[1024];
        long total_received = 0;

        while (total_received < filesize){
            long remaining = filesize - total_received; 
            size_t to_read = remaining < 1024 ? remaining : 1024; 
            ssize_t n;
            if ((n = tcp_read(active_socket, buf2, to_read)) <= 0){
                perror("read failed");
                break; 
            }

            fwrite(buf2, 1, n, output);
            total_received += n;
        }

        fclose(output); 
        printf("Saved %ld bytes to %s\n", total_received, actual_filename);
        close(active_socket); 
    }

    return 0;
}
