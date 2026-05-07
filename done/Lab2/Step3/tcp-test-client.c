#include <stdio.h> 
#include "error.h"
#include "socket_layer.h"
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

int main(void){

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
    
    unsigned int val; 
    printf("What int value do you want to send? ");
    scanf("%u", &val);

    char msg[64]; 
    snprintf(msg, sizeof(msg), "%u", val);
    printf("Sending message to %s:%u: %u\n", CS202_DEFAULT_IP, CS202_DEFAULT_PORT, val);
    tcp_send(sock, msg, strlen(msg));

    printf("Waiting for its reply.\n");
    char response[64] = {0}; 
    tcp_read(sock, response, sizeof(response)-1);

    printf("Received reply: %s\n", response); 
    close(sock); 
    return 0;
}
