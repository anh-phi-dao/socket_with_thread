#include "manage_client_with_thread.h"
#include "socket.h"
#include <sys/poll.h>

#define SERVER_IP "192.168.27.167"

char buff[1024];
int val_write;
int val_read;

int main()
{
    if (init_client_message_queue() == MQ_OPEN_ERROR)
    {
        return -1;
    }

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
    /*Request a file name, if this file exist in server, client will receive the data from the file*/

    if (pthread_create(manage_client_threads, NULL, request_file_thread, NULL) > 0)
    {
        perror("pthread_create");
        printf("Error on pthread\n");
        close(client_fd);
        mq_close(client_mq);
        mq_unlink(MANAGE_SERVER_QUEUE_NAME);
        exit(EXIT_FAILURE);
    }
    if (pthread_create(manage_client_threads + 1, NULL, client_transmit_recieve_thread, NULL) > 0)
    {

        perror("pthread_create");
        printf("Error on pthread\n");
        close(client_fd);
        mq_close(client_mq);
        mq_unlink(MANAGE_SERVER_QUEUE_NAME);
        exit(EXIT_FAILURE);
    }

    pthread_join(manage_client_threads[1], NULL);
    pthread_detach(manage_client_threads[0]);

    close(client_fd);
    mq_close(client_mq);
    mq_unlink(MANAGE_SERVER_QUEUE_NAME);

    return 0;
}