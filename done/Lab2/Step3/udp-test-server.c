#include <stdio.h>
#include "socket_layer.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h> // Ajout obligatoire pour gettimeofday()

// Structure de la sonde qui doit être identique côté client et serveur
typedef struct {
    uint32_t seq;
    long tv_sec;
    long tv_usec;
} probe_t;

int main (void){    
    int fd = udp_server_init(CS202_DEFAULT_IP, CS202_DEFAULT_PORT, 0);
    if (fd < 0) {
        fprintf(stderr, "Error intializing server\n");
        return 1;
    }

    printf("Server listening on %s:%u\n", CS202_DEFAULT_IP, CS202_DEFAULT_PORT); 

    // Variables pour stocker les statistiques sur 1 seconde
    struct timeval last_summary_time = {0, 0};
    long long sum_delay = 0;
    int count_delay = 0;

    while (1) {
        struct sockaddr_in client_addr = {0}; 
        probe_t probe; // On remplace le char array par notre structure
        
        // On lit directement la structure depuis le socket (on cast en char* au cas où udp_read le demande)
        int n = udp_read(fd, (char*)&probe, sizeof(probe), &client_addr);
        if (n < 0) {
            fprintf(stderr, "Error reading\n");
            close(fd);
            return 1;
        }

        // 1. On enregistre le temps exact de réception
        struct timeval recv_time;
        gettimeofday(&recv_time, NULL);

        // 2. On calcule le délai (Temps de réception - Temps d'envoi de la sonde) en microsecondes
        long long delay_us = (recv_time.tv_sec - probe.tv_sec) * 1000000LL + (recv_time.tv_usec - probe.tv_usec);
        
        printf("Probe %u: One Way Delay = %lld us.\n", probe.seq, delay_us);

        // 3. Logique pour la moyenne chaque seconde
        if (last_summary_time.tv_sec == 0) {
            last_summary_time = recv_time; // Initialisation à la première sonde reçue
        }

        sum_delay += delay_us;
        count_delay++;

        // Si une seconde (ou plus) s'est écoulée depuis le dernier résumé
        if (recv_time.tv_sec >= last_summary_time.tv_sec + 1) {
            double avg = (double)sum_delay / count_delay;
            printf("Average one-way delay of past 1s : %.2f us.\n", avg);
            
            // On reset les compteurs pour la seconde suivante
            sum_delay = 0;
            count_delay = 0;
            last_summary_time = recv_time;
        }
    }

    close(fd);
    return 0; 
}