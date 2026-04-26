/**
 * @file socket_layer.h
 * @brief Minimal socket layer.
 *
 * @author K. Prasopoulos
 * @author J.-C. Chappelier
 */

#pragma once

#include <stddef.h>    // size_t
#include <stdint.h>    // uint16_t
#include <sys/types.h> // ssize_t
#include <time.h>      // time_t

struct sockaddr_in; // in sys/socket.h

/**
 * @brief working on localhost by default
 */
#define CS202_DEFAULT_IP "127.0.0.1"

/**
 * @brief default port for communication
 */
#define CS202_DEFAULT_PORT 1234

/**
 * @brief Maximum value for a port number on Unix machine
 */
#define MAX_PORT_NUMBER ((1 << 16) - 1)

/** ======================================================================
 * @brief tool function to brutally exit on system error.
 * @param msg error message to be displayed.
 * @param socket socket where the error comes from.
 */
void die(const char* msg, int socket);

/**
 * @brief Converts an IPv4 string and a port number to a `struct sockaddr_in`.
 *
 * @param ip the IPv4 string in the format "X.X.X.X".
 * @param port the port number.
 * @param p_server_addr (out): a location where to write the result.
 * @return an error code (see error.h).
 */
int get_server_addr(const char* ip, uint16_t port,
                    struct sockaddr_in* p_server_addr);

/**
 * @brief Binds a socket to an IPv4 and port.
 *
 * @param socket the socket file descriptor.
 * @param ip the IPv4 string in the format "X.X.X.X".
 * @param port the port number.
 * @return an error code (see error.h).
 */
int bind_server(int socket, const char* ip, uint16_t port);

/**
 * @brief Creates a network UDP socket over IPv4.
 *
 * @param t if strictly positive, the receive timeout (in seconds) of the
 * socket. `0` means no timeout.
 * @return if positive, the socket file descriptor. Otherwise, an error code
 * (see error.h).
 */
int get_udp_socket(time_t t);

/**
 * @brief Creates a UDP socket bound to the given IPv4 and port.
 *
 * @param ip the IPv4 string in the format "X.X.X.X".
 * @param port the port number.
 * @param t if strictly positive, the receive timeout (in seconds) of the
 * socket. `0` means no timeout.
 * @return if positive, the socket file descriptor. Otherwise, an error code
 * (see error.h).
 */
int udp_server_init(const char* ip, uint16_t port, time_t t);
    

/**
 * @brief Reads at most `buflen` bytes from an UDP stream.
 *
 * @param socket a socket file descriptor to read from.
 * @param buf (out) a buffer where the data will be written.
 * @param buflen the size of the buffer.
 * @param cli_addr (out) if not null, the address of the sender will be written
 * here.
 * @return ssize_t if positive, the number of bytes received. Otherwise, an
 * error code (see error.h).
 */
ssize_t udp_read(int socket, char* buf, size_t buflen,
                 struct sockaddr_in* cli_addr);

/**
 * @brief Sends data over a UDP stream.
 *
 * @param socket a socket file descriptor to send from.
 * @param buf the data to send.
 * @param buflen the length in bytes of the data.
 * @param cli_addr the address of the recipient.
 * @return if positive, the number of bytes sent. Otherwise, an error code (see
 * error.h).
 */
ssize_t udp_send(int socket, const char* response, size_t response_len,
                 const struct sockaddr_in* cli_addr);

/**
 * @brief Creates a network TCP socket over IPv4.
 *
 * @return if positive, the socket file descriptor. Otherwise, an error code
 * (see error.h).
 */
int get_tcp_socket(void);

/**
 * @brief Creates a TCP socket bound to the given IPv4 and port.
 *
 * @param ip the IPv4 string in the format "X.X.X.X".
 * @param port the port number.
 * @return if positive, the socket file descriptor. Otherwise, an error code
 * (see error.h).
 */
int tcp_server_init(const char* ip, uint16_t port);

/**
 * @brief Blocking call that accepts a new TCP connection
 *
 * @param passive_socket the socket we are listing client requests from.
 * @param cli_addr (out) if not null, the address of the client will be written
 * here.
 * @return if positive, the socket to read from; otherwise, an error code
 * (see error.h).
 */
int tcp_accept(int passive_socket, struct sockaddr_in* cli_addr);

/**
 * @brief Blocking call that reads the active socket once and stores the output in buf
 *
 * @param active_socket a socket file descriptor to read from.
 * @param buf (out) a buffer where the data will be written.
 * @param buflen the size of the buffer.
 * @return ssize_t if positive, the number of bytes received. Otherwise, an
 * error code (see error.h).
 */
ssize_t tcp_read(int active_socket, char* buf, size_t buflen);

/**
 * @brief Sends data over a TCP stream.
 *
 * @param socket a socket file descriptor to send from.
 * @param buf the data to send.
 * @param buflen the length in bytes of the data.
 * @param cli_addr the address of the recipient.
 * @return if positive, the number of bytes sent. Otherwise, an error code (see
 * error.h).
 */
ssize_t tcp_send(int active_socket, const char* response, size_t response_len);
