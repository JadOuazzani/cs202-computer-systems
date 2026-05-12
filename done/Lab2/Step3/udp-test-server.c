#include <stdio.h>
#include "socket_layer.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>  
#include <time.h> 

typedef struct {
    int id;
    long sec;
    long usec; 
} Probe;

int main (void){    
    int fd = udp_server_init(CS202_DEFAULT_IP, CS202_DEFAULT_PORT, 0);
    if (fd < 0) {
        fprintf(stderr, "Error intializing server\n");
        return 1;
    }

    printf("Server listening on %s:%u\n", CS202_DEFAULT_IP, CS202_DEFAULT_PORT); 


    struct timeval start = {0};
    gettimeofday(&start, NULL);
    struct timeval end = {0};
    gettimeofday(&end, NULL);

    while (1) {
        start = end; 
        time_t total = 0;
        int nbr = 0;
        while (end.tv_sec - start.tv_sec < 1){
            struct sockaddr_in client_addr = {0}; 
            Probe probe = {0}; 
            int n = udp_read(fd, (char*)&probe, sizeof(probe) , &client_addr);
            if (n < 0) {
                fprintf(stderr, "Error reading\n");
                close(fd);
                return 1;
            }

            struct timeval tv_recv = {0};
            gettimeofday(&tv_recv, NULL);
        
            time_t dur = (tv_recv.tv_sec  - probe.sec)  * 1000000 + tv_recv.tv_usec - probe.usec ; 
            printf("Probe %d: One way Delay = %ld us.\n", probe.id, dur);
            total += dur;
            nbr += 1; 
            
            gettimeofday(&end, NULL);
        }
        float average = nbr > 0 ? total / (float) nbr : 0;
        printf("Average one-way delay of past 1s : %f us.\n", average);
        
    } 
    close(fd);
    return 0;
}
