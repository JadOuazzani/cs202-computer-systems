#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <signal.h>

#include "socket_layer.h"
#include "error.h"
#include "util.h"
// --- ÉTAPE 5 : Système de lock par nom de fichier ---
#ifndef NAME_MAX
#define NAME_MAX 255
#endif
#define NB_FILES_MAX 128

typedef struct {
    char filename[NAME_MAX+1];
    pthread_mutex_t mutex;
} filename_lock_t;

filename_lock_t mutexes[NB_FILES_MAX];
pthread_mutex_t hashtable_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t* get_filename_mutex(const char* filename) {
    pthread_mutex_lock(&hashtable_mutex);
    
    int i = 0;
    // Recherche linéaire
    while (i < NB_FILES_MAX) {
        if (mutexes[i].filename[0] == '\0' || strcmp(mutexes[i].filename, filename) == 0) {
            break;
        }
        i++;
    }
    
    // Si on dépasse, on revient à 0 (logique demandée dans le lab)
    if (i == NB_FILES_MAX) {
        i = 0; 
    }

    if (strcmp(mutexes[i].filename, filename) != 0) {
        strncpy(mutexes[i].filename, filename, NAME_MAX);
        mutexes[i].filename[NAME_MAX] = '\0';
        pthread_mutex_init(&mutexes[i].mutex, NULL);
        
        // Préparer la prochaine case à être vide
        int next = (i + 1) % NB_FILES_MAX;
        mutexes[next].filename[0] = '\0';
    }
    
    pthread_mutex_t* target = &mutexes[i].mutex;
    pthread_mutex_unlock(&hashtable_mutex);
    
    return target;
}

// --- ÉTAPE 3.3 : Structure pour les arguments du thread ---
typedef struct {
    int active_socket;
    struct sockaddr_in cli_addr;
} thread_args_t;


void* handle_connection(void* args) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

    // Récupération des arguments
    thread_args_t* t_args = (thread_args_t*)args;
    int active_socket = t_args->active_socket;
    struct sockaddr_in addr = t_args->cli_addr;
    free(t_args);

    printf("[Thread %lu] Connexion gérée.\n", pthread_self());

    // Lire le nom de fichier
    char filename[NAME_MAX+1] = {0};
    if (tcp_read(active_socket, filename, NAME_MAX) < 0) {
        close(active_socket); return NULL;
    }
    printf("[Thread %lu] Receiving file: %s\n", pthread_self(), filename);

    // Lock sur ce fichier
    pthread_mutex_t* file_mutex = get_filename_mutex(filename);
    pthread_mutex_lock(file_mutex);

    // Ouvrir et écrire le fichier
    FILE* file = fopen(filename, "wb");
    size_t total_written = 0;
    char buf[1024];
    ssize_t n;
    while ((n = tcp_read(active_socket, buf, sizeof(buf))) > 0) {
        fwrite(buf, 1, n, file);
        total_written += n;
    }
    fclose(file);

    sleep(5); // délai artificiel
    printf("[Thread %lu] %zu bytes wrote to \"%s\".\n",
           pthread_self(), total_written, filename);

    pthread_mutex_unlock(file_mutex);

    // Envoyer l'ACK (taille écrite)
    char ack[32];
    snprintf(ack, sizeof(ack), "%zu", total_written);
    tcp_send(active_socket, ack, strlen(ack));

    close(active_socket);
    return NULL;
}

int main (int argc, char* argv[]) {
    // ÉTAPE 2.3 : Choix du port depuis la ligne de commande
    uint16_t port = CS202_DEFAULT_PORT;
    if (argc > 1) {
        port = atouint16(argv[1]);
        if (port == 0) {
            fprintf(stderr, "Invalid port number.\n");
            return 1;
        }
    }

    int passive_socket = tcp_server_init(CS202_DEFAULT_IP, port); 
    if (passive_socket < 0 ) {
        perror("server init failed");
        return 1;
    }
    printf("Server listening on %s:%u\n", CS202_DEFAULT_IP, port);

    // Initialiser la première case du tableau de mutex à vide
    mutexes[0].filename[0] = '\0';

    while (1) {
        struct sockaddr_in addr; 
        int active_socket = tcp_accept(passive_socket, &addr);
        if (active_socket < 0) {
            perror("accept failed");
            continue; // Mieux vaut continuer que crasher le serveur
        }
        
        // Allocation des arguments sur le tas (heap) pour éviter les race conditions
        thread_args_t* args = malloc(sizeof(thread_args_t));
        if (!args) {
            perror("malloc failed");
            close(active_socket);
            continue;
        }
        args->active_socket = active_socket;
        args->cli_addr = addr;

        // ÉTAPE 4.1 : Création du thread détaché
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        pthread_t tid;
        if (pthread_create(&tid, &attr, handle_connection, args) != 0) {
            perror("pthread_create failed");
            free(args);
            close(active_socket);
        }
        
        pthread_attr_destroy(&attr);
    }

    return 0;
}