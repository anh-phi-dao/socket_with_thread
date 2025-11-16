#include "client.h"
#include "socket.h"
#include <mqueue.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/select.h>
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

/**
 * @brief init the message queue for the client
 * @return MQ_OPEN_SUCCESS=0 when succes, MQ_OPEN_ERROR = -1
 * when failing
 */
int init_client_message_queue();
/**
 * @brief Thread that request user to enter the name of the file and send the
 * file name to second threads through message queue
 */
void *request_file_thread(void *arg);
/**
 * @brief Receive file name from message queue, send the file name to server and
 * read the result
 */
void *client_transmit_recieve_thread(void *arg);