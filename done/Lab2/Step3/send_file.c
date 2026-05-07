#include <stdio.h> 
#include "error.h"
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include "socket_layer.h"

#define MAX_FILE_NAME 1024
#define BUFFER_SIZE 1024

int main(void){
    char filename[MAX_FILE_NAME] = {0}; 
    printf("What file do you want to send? \n");
    scanf("%s", filename);

    FILE* input = fopen(filename, "r");
    if (input == NULL){
        fprintf(stderr, "Error: cannot open file \"%s\" in read mode!\n", filename);
        return ERR_NOT_FOUND;
    }

    int sock = get_tcp_socket();
    if (sock < 0) return ERR_NETWORK;

    struct sockaddr_in addr;  
    get_server_addr(CS202_DEFAULT_IP, CS202_DEFAULT_PORT, &addr);
    
    if (connect(sock, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
        perror("connect failed");
        close(sock); 
        return ERR_NETWORK;
    }
    printf("Connected to %s:%u\n", CS202_DEFAULT_IP, CS202_DEFAULT_PORT);

    //FILENAME
    char msg[MAX_FILE_NAME + 16]; 
    snprintf(msg, sizeof(msg), "FILE: %s", filename); 
    printf("Sending filename to %s:%u: %s\n", CS202_DEFAULT_IP, CS202_DEFAULT_PORT, filename);
    tcp_send(sock, msg, strlen(msg)+1);

    //FILESIZE
    fseek(input, 0, SEEK_END); 
    long filesize = ftell(input); 
    fseek(input, 0, SEEK_SET); 
    char msg2[64] = {0};
    snprintf(msg2, sizeof(msg2), "%ld|", filesize); 
    printf("Sending file size to %s:%u: %ld\n", CS202_DEFAULT_IP, CS202_DEFAULT_PORT, filesize);
    tcp_send(sock, msg2, strlen(msg2));

    //FILECONTENT
    char buf[BUFFER_SIZE]; 
    size_t byte_read; 
    while ((byte_read = fread(buf, 1, BUFFER_SIZE , input)) != 0){
        tcp_send(sock, buf, byte_read);
    }

    fclose(input); 
    close(sock); 
    return 0;
}
