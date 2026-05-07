#include <stdio.h>
#include "socket_layer.h"
#include "error.h"
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

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
        char buf[64] = {0};
        if (tcp_read(active_socket, buf, sizeof(buf)-1) < 0){
            perror("accept failed");
            return ERR_NETWORK; ;
        }
        printf("Received message from %s:%u: %s\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buf); 

        unsigned int val; 
        sscanf(buf, "%u", &val); 
        val += 1;
        char msg[64]; 
        snprintf(msg, sizeof(msg), "%u", val);
        printf("Sending message to %s:%u: %u\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), val); 
        tcp_send(active_socket, msg, strlen(msg));
        close(active_socket); 
    }

    return 0;
}
