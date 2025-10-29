#include "client.h"

struct sockaddr_in server_address;

socklen_t len = (socklen_t)sizeof(struct sockaddr_in);

int client_fd;

int create_TCP_IPv4_client(int *client_fd, socklen_t *len)
{
    *client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*client_fd < 0)
    {
        *client_fd = 0;
        printf("Failed on socket function\n");
        return ERROR;
    }

    printf("Successfully creating TCP client\n");
    return SUCCESS;
}

int connect_to_TCP_IPv4_server(struct sockaddr_in *server_addr, char *IPv4, uint16_t port, int *client_fd, socklen_t *len)
{
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    if (inet_pton(AF_INET, IPv4, &server_addr->sin_addr) <= 0)
    {
        printf("Invalid address, cancel creating server\n");
        return ERROR;
    }

    if (connect(*client_fd, (struct sockaddr *)server_addr, *len) == -1)
    {
        printf("Connection to server has failed\n");
        return ERROR;
    }

    printf("Successfully connecting to TCP server\n");
    return SUCCESS;
}