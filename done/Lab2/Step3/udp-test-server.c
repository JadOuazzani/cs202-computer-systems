#include <stdio.h>
#include "socket_layer.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>



int main (void){    
    int fd = udp_server_init(CS202_DEFAULT_IP, CS202_DEFAULT_PORT, 0);
    if (fd < 0) {
        fprintf(stderr, "Error intializing server\n");
        return 1;
    }

    printf("Server listening on %s:%u\n", CS202_DEFAULT_IP, CS202_DEFAULT_PORT); 

    while (1) {
        struct sockaddr_in client_addr = {0}; 
        char response[64] = {0}; 
        int n = udp_read(fd, response, sizeof(response)-1 , &client_addr);
        if (n < 0) {
            fprintf(stderr, "Error reading\n");
            close(fd);
            return 1;
        }
        printf("Received message from %s:%u: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), response);
        
        unsigned int val;
        sscanf(response, "%u", &val); 
        val += 1; 
        snprintf(response, sizeof(response), "%u", val); 
        printf("Sending message to %s:%u: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), response); 
        udp_send(fd, response, strlen(response), &client_addr);
    }

    close(fd);
    return 0; 
}