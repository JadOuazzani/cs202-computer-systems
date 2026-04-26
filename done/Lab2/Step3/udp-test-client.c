#include <stdio.h>
#include "socket_layer.h"
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main (void){
    unsigned int val; 
    printf("What int value do you want to send? "); 
    scanf("%u", &val);

    int fd = get_udp_socket(5);

    struct sockaddr_in server_addr = {0}; 
    get_server_addr(CS202_DEFAULT_IP, CS202_DEFAULT_PORT, &server_addr); 

    char msg[64];
    snprintf(msg, sizeof(msg), "%u", val);

    printf("Sending message to 127.0.0.1:1234: %u\n", val); 
    udp_send(fd, msg, strlen(msg), &server_addr); 

    char response[64] = {0}; 
    struct sockaddr_in from_addr;
    udp_read(fd, response, sizeof(response)-1, &from_addr); 
    
    printf("Received response: %s\n", response); 
    close(fd);
    return 0;
}