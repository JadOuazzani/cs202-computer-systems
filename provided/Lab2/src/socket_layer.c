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

        // ...to be continued...

    return ERR_NONE;
}

// ======================================================================
int bind_server(int socket, const char* ip, uint16_t port)
{
    M_REQUIRE_NON_NULL(ip);

    struct sockaddr_in server_addr = {0};

    int err = ERR_NONE;
    // ...to be continued...

    return err;
}

// ======================================================================
int get_udp_socket(time_t t)
{
    const int fd = -1; // to be modified

    // ...to be continued...

    if (t > 0) {
        // Set receive timeout
        struct timeval timeout = {0};
        // ...to be continued...
    }

    return fd;
}

// ======================================================================
int udp_server_init(const char* ip, uint16_t port, time_t t)
{
    M_REQUIRE_NON_NULL(ip);

    // Create a socket
    const int socket = -1; // to be modified
    if (socket < 0) {
        perror("Error opening UDP socket");
        return ERR_NETWORK;
    }

    // Bind the socket to the address
    // ...to be continued...

    return socket;
}

// ======================================================================
ssize_t udp_read(int socket, char* buf, size_t buflen,
                 struct sockaddr_in* cli_addr)
{
    M_REQUIRE_NON_NULL(buf);
    M_REQUIRE_NON_NULL(cli_addr);

    const ssize_t res = -1; // to be modified

    return res < 0 ? ERR_NETWORK : res;
}

// ======================================================================
ssize_t udp_send(int socket, const char* response, size_t response_len,
                 const struct sockaddr_in* cli_addr)
{
    M_REQUIRE_NON_NULL(response);
    M_REQUIRE_NON_NULL(cli_addr);

    const ssize_t res = -1; // to be modified

    return res < 0 ? ERR_NETWORK : res;
}

// ======================================================================
int get_tcp_socket(void)
{
    const int fd = -1; // to be modified

    // ...to be continued...

    return fd;
}

// ======================================================================
int tcp_server_init(const char* ip, uint16_t port)
{
    M_REQUIRE_NON_NULL(ip);

    // Create a socket
    const int socket = -1; // to be modified
    if (socket < 0) {
        perror("Error opening UDP socket");
        return ERR_NETWORK;
    }

    // Bind the socket to the address
    // ...to be continued...

    // Start listening for incoming connections
    // ...to be continued...

    return socket;
}

// ======================================================================
int tcp_accept(int passive_socket,
                 struct sockaddr_in* cli_addr)
{
    M_REQUIRE_NON_NULL(cli_addr);

    return -1; // to be modified
}

// ======================================================================
ssize_t tcp_read(int socket, char* buf, size_t buflen)
{
    M_REQUIRE_NON_NULL(buf);

    const ssize_t res = -1; // to be modified

    return res < 0 ? ERR_NETWORK : res;
}

// ======================================================================
ssize_t tcp_send(int socket, const char* response, size_t response_len)
{
    M_REQUIRE_NON_NULL(response);

    const ssize_t res = -1; // to be modified

    return res < 0 ? ERR_NETWORK : res;
}
