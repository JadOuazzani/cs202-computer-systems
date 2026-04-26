#include <stdio.h>
#include "socket_layer.h"

int main (void){
    unsigned int val; 
    printf("What int value do you want to send?"); 
    scanf("%u", &val);

    int fd = udp_server_init(&CS202_DEFAULT_IP, CS202_DEFAULT_PORT, 5); 
    
    
    
    
}