#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "error.h"
#include "socket_layer.h"
#include "util.h"

int main(int argc, char* argv[]) {
    
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

    // Lire le nom de fichier depuis stdin
    char filename[256] = {0};
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Erreur de lecture du nom de fichier.\n");
        close(sock);
        return 1;
    }

    // Ouvrir le fichier
    FILE* f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        close(sock);
        return 1;
    }

    // Envoyer le nom de fichier
    tcp_send(sock, filename, strlen(filename));

    // Envoyer le contenu
    char buf[1024];
    size_t total_sent = 0;
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
        tcp_send(sock, buf, n);
        total_sent += n;
    }
    fclose(f);

    // Signaler la fin d'envoi
    shutdown(sock, SHUT_WR);
    printf("Sent %zu bytes.\n", total_sent);

    // Attendre l'ACK et vérifier
    char ack[32] = {0};
    if (tcp_read(sock, ack, sizeof(ack)-1) > 0) {
        size_t ack_val = (size_t)strtoul(ack, NULL, 10);
        if (ack_val == total_sent)
            printf("ACK OK: server wrote %zu bytes.\n", ack_val);
        else
            printf("ACK MISMATCH: sent %zu, server wrote %zu.\n", total_sent, ack_val);
    } else {
        printf("Failed to receive ACK.\n");
    }

    close(sock); 
    return 0;
}