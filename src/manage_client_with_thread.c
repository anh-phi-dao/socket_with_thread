#include "manage_client_with_thread.h"

mqd_t client_mq;
struct mq_attr attr;
pthread_t manage_client_threads[NUMBER_OF_THREAD];
pthread_mutex_t close_mutex = PTHREAD_MUTEX_INITIALIZER;
char close_state[20];

/*init message queue*/
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

/*Read from STANDARD INPUT and send file request to server*/
void *request_file_thread(void *arg)
{
    char file_name[100];
    fd_set read_sel;
    struct timeval time;
    time.tv_sec = 0;
    time.tv_usec = 1000;
    int ret;
    while (1)
    {
        for (int i = 0; i < 100; i++)
        {
            file_name[i] = 0;
        }
        write(STDOUT_FILENO, "\nPlease enter the file: ", 25);
        /*check the standard input*/
    check_again:
        FD_ZERO(&read_sel);
        FD_SET(STDIN_FILENO, &read_sel);
        FD_SET(STDOUT_FILENO, &read_sel);
        ret = select(1, &read_sel, NULL, NULL, &time);
        if (ret == -1)
        {
            perror("Error on select");
            return NULL;
        }
        /*if input has data ready to read, then send to tcp thread through queue*/
        ret = FD_ISSET(STDIN_FILENO, &read_sel);
        if (ret != 0)
        {
            scanf("\n");
            scanf("%100[^\n]s", file_name);
            if (mq_send(client_mq, file_name, strlen(file_name), 0) == -1)
            {
                perror("mq send");
                printf("Request file thread with id:%lu\n", pthread_self());
                return NULL;
            }
            sleep(1);
        }
        /*if there is close message from client, close the client*/
        pthread_mutex_lock(&close_mutex);
        if (strcmp(close_state, "Close") == 0)
        {
            printf("Closing this client\n");
            pthread_mutex_unlock(&close_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&close_mutex);
        if (ret == 0)
        {
            goto check_again;
        }
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
        memset(file_name, 0, strlen(file_name));
        memset(buff, 0, strlen(buff));
        /*if receive name*/
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

        val_read = read(client_fd, buff, 1024);
        if (val_read > 0)
        {
            printf("Num of bytes read %d\n", val_read);
        }
        else if (val_read == 0)
        {
            printf("Server has closed\n");
            pthread_mutex_lock(&close_mutex);
            sprintf(close_state, "Close");
            pthread_mutex_unlock(&close_mutex);
            close(client_fd);
            return NULL;
        }

        printf("Read %s from server:\n\n%s", file_name, buff);
    }
    return NULL;
}
