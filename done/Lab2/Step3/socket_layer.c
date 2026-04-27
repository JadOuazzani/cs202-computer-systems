/*
 * @file socket_layer.c
 * @brief socket binding for CS-202 project
 *
 */

#include "socket_layer.h"

#include "error.h"

#include <errno.h> // for errno
#include <stdlib.h> // for exit
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h> // for struct timeval
#include <unistd.h>

// ======================================================================
void die(const char* msg, int socket)
{
    if (errno)
        perror(msg);
    else
        fprintf(stderr, "%s failed for some unreported reason.\n", msg);

    if (socket >= 0) close(socket);
    exit(1);
}

// ======================================================================
int get_server_addr(const char* ip, uint16_t port,
                    struct sockaddr_in* p_server_addr)
{
    M_REQUIRE_NON_NULL(ip);
    M_REQUIRE_NON_NULL(p_server_addr);

    struct sockaddr_in server_addr = {0};
    if (inet_pton(AF_INET, ip, &(server_addr.sin_addr)) != 1) return ERR_ADDRESS; 
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET; 
    
    *p_server_addr = server_addr;

    return ERR_NONE;
}

// ======================================================================
int bind_server(int socket, const char* ip, uint16_t port)
{
    M_REQUIRE_NON_NULL(ip);

    struct sockaddr_in server_addr = {0};
    int err = get_server_addr(ip, port, &server_addr);
    if (err != ERR_NONE) return err;

    if (bind(socket, ((struct sockaddr *)& server_addr), sizeof(server_addr)) != 0) return ERR_NETWORK; ;

    return err;
}

// ======================================================================
int get_udp_socket(time_t t)
{
    const int fd = socket(AF_INET, SOCK_DGRAM, 0); 

    if (fd == -1) return ERR_NETWORK; 

    if (t > 0) {
        // Set receive timeout
        struct timeval timeout = {0};
        timeout.tv_sec = t; 

        if (setsockopt(fd, SOL_SOCKET ,SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0){
            close(fd); 
            return ERR_NETWORK; 
        }
    }

    return fd;
}

// ======================================================================
int udp_server_init(const char* ip, uint16_t port, time_t t)
{
    M_REQUIRE_NON_NULL(ip);

    // Create a socket
    const int socket = get_udp_socket(t); 
    if (socket < 0) {
        perror("Error opening UDP socket");
        return ERR_NETWORK;
    }

    int err = bind_server(socket, ip, port);
    if ( err < 0 ) {
        close(socket);
        return err; 
    }

    return socket;
}

// ======================================================================
ssize_t udp_read(int socket, char* buf, size_t buflen,
                 struct sockaddr_in* cli_addr)
{
    M_REQUIRE_NON_NULL(buf);
    M_REQUIRE_NON_NULL(cli_addr);
    socklen_t addr_len = sizeof(*cli_addr);
    const ssize_t res = recvfrom(socket, buf, buflen, 0, (struct sockaddr *) cli_addr, &addr_len); 

    return res < 0 ? ERR_NETWORK : res;
}

// ======================================================================
ssize_t udp_send(int socket, const char* response, size_t response_len,
                 const struct sockaddr_in* cli_addr)
{
    M_REQUIRE_NON_NULL(response);
    M_REQUIRE_NON_NULL(cli_addr);

    const ssize_t res = sendto(socket, response, response_len, 0, (struct sockaddr *)cli_addr, sizeof(*cli_addr)); 

    return res < 0 ? ERR_NETWORK : res;
}

// ======================================================================
int get_tcp_socket(void)
{
    const int fd = socket(AF_INET, SOCK_STREAM, 0); 

    if (fd < 0) {
       return ERR_NETWORK;  
    }

    return fd;
}

// ======================================================================
int tcp_server_init(const char* ip, uint16_t port)
{
    M_REQUIRE_NON_NULL(ip);

    // Create a socket
    const int socket = get_tcp_socket(); 
    if (socket < 0) {
        perror("Error opening UDP socket");
        return ERR_NETWORK;
    }

    // Bind the socket to the address
    if (bind_server(socket, ip, port) < 0) {
        perror("Error binding UDP socket");
        close(socket); 
        return ERR_NETWORK; 
    }

    // Start listening for incoming connections
    if (listen(socket, 5) != 0 ) {
        perror("Error listenning");
        close(socket); 
        return ERR_NETWORK; 
    };

    return socket;
}

// ======================================================================
int tcp_accept(int passive_socket,
                 struct sockaddr_in* cli_addr)
{
//  M_REQUIRE_NON_NULL(cli_addr);
    socklen_t addr_len = sizeof(*cli_addr);
    return cli_addr == NULL ? accept(passive_socket, NULL, NULL) : accept(passive_socket, (struct sockaddr*) cli_addr, &addr_len); 
}

// ======================================================================
ssize_t tcp_read(int socket, char* buf, size_t buflen)
{
    M_REQUIRE_NON_NULL(buf);

    if (socket < 0) {
        return ERR_INVALID_ARGUMENT; 
    }

    const ssize_t res = recv(socket, buf, buflen, 0); 

    return res < 0 ? ERR_NETWORK : res;
}

// ======================================================================
ssize_t tcp_send(int socket, const char* response, size_t response_len)
{
    M_REQUIRE_NON_NULL(response);

    if (socket < 0) {
        return ERR_INVALID_ARGUMENT; 
    }
    const ssize_t res = send(socket, response, response_len, 0);

    return res < 0 ? ERR_NETWORK : res;
}
