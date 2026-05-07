#include <stdio.h>
#include "socket_layer.h"
#include <netinet/in.h>
#include <string.h>
#include <sys/time.h>  
#include <time.h> 
#include <unistd.h>

typedef struct {
    int id;
    long sec;
    long usec; 
} Probe; 

int main (void){
    int fd = get_udp_socket(5);
    struct sockaddr_in server_addr = {0}; 
    get_server_addr(CS202_DEFAULT_IP, CS202_DEFAULT_PORT, &server_addr);
    printf("Connected to %s:%u\n", CS202_DEFAULT_IP, CS202_DEFAULT_PORT);

    int id = 0; 
    while(1){
        struct timeval tv = {0};
        gettimeofday(&tv, NULL); 

        Probe probe; 
        probe.id   = id;
        probe.sec  = tv.tv_sec;
        probe.usec = tv.tv_usec;
    
        udp_send(fd, (char*)&probe, sizeof(Probe), &server_addr); 
        printf("Sent Probe %d.\n", id++);
        
        usleep(5000);
    }
    
    close(fd);
    return 0;
}
