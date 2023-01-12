#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/msg.h>
#include <stdlib.h>

#include "define/message.h"
#include "define/sem.c"
#include "sender/sender.h"
#include "receiver/receiver.h"

pthread_t sender1d, sender2d, received;

#define INIT_SEM(sem_return_status, sem_name, init_value)           \
    printf("Init Sem ....\n");                                      \
    sem_return_status = sem_init(&sem_name, 0, init_value);         \
    if (sem_return_status < 0)                                      \
    {                                                               \
        printf("sem init fail with code %d \n", sem_return_status); \
        exit(-1);                                                   \
    }

#define debugIO(a) printf("%s",a);

int main()
{

    // init sems
    int sem_return_status;
    INIT_SEM(sem_return_status, send, 1)
    INIT_SEM(sem_return_status, recv, 0)
    INIT_SEM(sem_return_status, over1, 0)
    INIT_SEM(sem_return_status, over2, 0)

    msgqid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT); // 得到消息队列标识符或创建一个消息队列对象
                                                    // 0666 任何人可写
    printf("msgqid: %d\n", msgqid);
    if (msgqid < 0)
    {
        printf("msgget: failed!\n");
        exit(1);
    }

    pthread_create(&sender1d, NULL, sender1, NULL); // 启动sender1线程
    pthread_create(&sender2d, NULL, sender2, NULL); // 启动sender2线程
    pthread_create(&received, NULL, receive, NULL); // 启动receiver线程

    pthread_join(sender1d, NULL); // 等待线程结束
    //debugIO("sender1");
    pthread_join(sender2d, NULL); // 等待线程结束
    //debugIO("sender2");
    pthread_join(received, NULL); // 等待线程结束
    //debugIO("received");

    return 0;
}