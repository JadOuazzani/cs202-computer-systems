#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "error.h"
#include "socket_layer.h"
#include "util.h" // Pour atouint16

int main(int argc, char* argv[]) {
    
    // ÉTAPE 2.3 : Choix du port depuis la ligne de commande
    uint16_t port = CS202_DEFAULT_PORT;
    if (argc > 1) {
        port = atouint16(argv[1]);
        if (port == 0) {
            fprintf(stderr, "Invalid port number.\n");
            return 1;
        }
    }

    int sock = get_tcp_socket();
    if (sock < 0) return ERR_NETWORK;

    struct sockaddr_in addr;  
    get_server_addr(CS202_DEFAULT_IP, port, &addr);
    
    if (connect(sock, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
        perror("connect failed");
        close(sock); 
        return ERR_NETWORK;
    }
    printf("Connected to %s:%u\n", CS202_DEFAULT_IP, port);
    
    unsigned int val; 
    printf("What int value do you want to send? ");
    if (scanf("%u", &val) != 1) {
        fprintf(stderr, "Erreur de lecture de l'entier.\n");
        close(sock);
        return 1;
    }

    char msg[64]; 
    snprintf(msg, sizeof(msg), "%u", val);
    printf("Sending message to %s:%u: %u\n", CS202_DEFAULT_IP, port, val);
    tcp_send(sock, msg, strlen(msg));

    // ÉTAPE 2.1 : Attente de l'acquittement
    printf("Waiting for server acknowledgment (with simulated delay)...\n");
    char response[64] = {0}; 
    if (tcp_read(sock, response, sizeof(response)-1) > 0) {
        printf("Received reply (ack): %s\n", response); 
    } else {
        printf("Failed to receive reply.\n");
    }
    
    close(sock); 
    return 0;
}