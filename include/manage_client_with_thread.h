#include "client.h"
#include "socket.h"
#include <mqueue.h>
#include <unistd.h>
#include <pthread.h>

#define MANAGE_SERVER_QUEUE_NAME "/server_queue"
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 1024
#define NUMBER_OF_THREAD 2

extern mqd_t client_mq;
extern struct mq_attr attr;
extern pthread_t manage_client_threads[NUMBER_OF_THREAD];

enum InitQueueState
{
    MQ_OPEN_ERROR = -1,
    MQ_OPEN_SUCCESS
};

int init_client_message_queue();
void *request_file_thread(void *arg);
void *client_transmit_recieve_thread(void *arg);