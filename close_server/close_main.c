#include "client.h"
#include "socket.h"
#include <sys/poll.h>

#define SERVER_IP "192.168.27.167"

char buff[1024];
char file_name[100];
int val_write;
int val_read;

int main()
{
    /*create an TCP client using IPv4 address*/
    if (create_TCP_IPv4_client(&client_fd, &len) == ERROR)
    {
        return -1;
    }
    /*identify the server IPv4 address and server's port*/
    if (connect_to_TCP_IPv4_server(&server_address, SERVER_IP, PORT, &client_fd, &len) == ERROR)
    {
        return -1;
    }
    /*Send close message so the server will automatically close, which means other clients will close as well*/
    /*After server has close, server send a FIN command*/
    /*If client executes read command and the return result is 0, this means server has completely closed*/
    do
    {
        val_write = writen(client_fd, "Close", 6);
        val_read = read(client_fd, buff, 1024);
        printf("%s", buff);
        sleep(1);
    } while (val_read > 0);

    /*Terminate the client*/
    close(client_fd);

    return 0;
}