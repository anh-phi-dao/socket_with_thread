#include "manage_client_with_thread.h"

mqd_t client_mq;
struct mq_attr attr;
pthread_t manage_client_threads[NUMBER_OF_THREAD];

int init_client_message_queue()
{
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    mq_unlink(MANAGE_SERVER_QUEUE_NAME);

    client_mq = mq_open(MANAGE_SERVER_QUEUE_NAME, O_CREAT | O_RDWR, 0666, &attr);

    if (client_mq == (mqd_t)-1)
    {
        perror("init_server_message_queue->mq_open");
        return MQ_OPEN_ERROR;
    }

    return MQ_OPEN_SUCCESS;
}

void *request_file_thread(void *arg)
{
    char file_name[100];

    while (1)
    {
        for (int i = 0; i < 100; i++)
        {
            file_name[i] = 0;
        }
        printf("Please enter the file: ");
        scanf("%s", file_name);

        if (mq_send(client_mq, file_name, strlen(file_name), 0) == -1)
        {
            perror("mq send");
            printf("Request file thread with id:%lu\n", pthread_self());
            return NULL;
        }
        sleep(1);
    }
    return NULL;
}

void *client_transmit_recieve_thread(void *arg)
{
    char file_name[100];
    char buff[1024];
    int val_write;
    int val_read;
    while (1)
    {
        for (int i = 0; i < 100; i++)
        {
            file_name[i] = 0;
            buff[i] = 0;
        }
        for (int i = 100; i < 1024; i++)
        {
            buff[i] = 0;
        }
        if (mq_receive(client_mq, file_name, MAX_MSG_SIZE, NULL) == -1)
        {
            perror("mq send");
            printf("client_transmit_recieve_thread id:%lu\n", pthread_self());
            return NULL;
        }
        val_write = writen(client_fd, file_name, strlen(file_name));
        if (val_write != strlen(file_name))
        {
            printf("File name is :%s\n", file_name);
            printf("strlen of gfile name is :%lu\n", strlen(file_name));
            printf("Send file name to server has failed, please send again\n");
            continue;
        }
        else
        {
            printf("\nNumber of bytes have been sent :%d\n\n", val_write);
        }
        /*Read the TCP socket, if there are data or message from server, print out to terminal*/
        /*If the number of characters read from socket is 0, this means the server has closed*/
        for (int i = 0; i < 5; i++)
        {

            val_read = read(client_fd, buff, 1024);
            if (val_read > 0)
            {
                printf("Num of bytes read %d\n", val_read);
                break;
            }
            else if (i == 4 && val_read == 0)
            {
                printf("Server has closed\n");
                close(client_fd);
                return NULL;
            }
            sleep(1);
        }

        printf("Read %s from server:\n\n%s", file_name, buff);
    }
    return NULL;
}
