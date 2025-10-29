#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*libray for socket structure*/
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8000
#define MAXIMUM_CLIENT 1

extern struct sockaddr_in server_address;
extern socklen_t len;
extern int client_fd;

/**
 * @brief Create a TCP client
 * @param fd: A pointer that points to the file descriptor of the client socket. User can use predefined int client_fd.
 * The function will create a file descriptor the bond to the TCP client,
 * @param len: a pointer to socken_t which store the length of the address (IPv4 is different, IPv6 is different)
 * . Users can use predefined socklen_t len;
 * @return SUCCESS=0, ERROR=-1
 */
int create_TCP_IPv4_client(int *client_fd, socklen_t *len);
/**
 * @brief Connect a TCP server, user must know which PORT, IPv4 address of that server
 * @param server_addr: pointer to server address following IPv4, the function will create a server that can received
 * any client address and the information will be stored in server address. Users can use predefined struct sockaddr_in server_address
 * @param port: port number, this is a parameter belongs to transport layer of OSI model. User can use predefined PORT =8000
 * @param client_fd: A pointer that points to the file descriptor of the client socket. User can use predefined int client_fd
 * @param len: a pointer to socken_t which store the length of the address (IPv4 is different, IPv6 is different)
 * . Users can use predefined socklen_t len;
 * @return SUCCESS=0, ERROR=-1
 */
int connect_to_TCP_IPv4_server(struct sockaddr_in *server_addr, char *IPv4, uint16_t port, int *client_fd, socklen_t *len);

enum
{
    ERROR = -1,
    SUCCESS
};