# include<stdio.h>
# include<string.h>
# include<sys/ipc.h>
# include<pthread.h>
# include<semaphore.h>
# include<sys/msg.h>
# include<stdlib.h>

#include "define/message.h"
#include "define/sem.c"
#include "sender/sender.h"
#include "receiver/receiver.h"

pthread_t sender1d, sender2d, received;

int main()
{
    /*
        主函数做的工作：
        1. 初始化信号量
        2. 创建消息队列
        3. 创建三个线程并执行
        4. 线程结束后扫尾工作
    */
    int sem_status;
    sem_status = sem_init(&send,0,1); //初始化send信号量, 0表示进程内多线程共享, 1表示初始值为1.
    if (sem_status < 0) {
        printf("sem send init fail with code %d", sem_status);
        exit(0);
    }
    sem_status = sem_init(&recv,0,0);
    if (sem_status < 0) {
        printf("sem recv init fail with code %d", sem_status);
        exit(0);
    }
    sem_status = sem_init(&over1,0,0);
    if (sem_status < 0) {
        printf("sem over1 init fail with code %d", sem_status);
        exit(0);
    }
    sem_status = sem_init(&over2,0,0);
    if (sem_status < 0) {
        printf("sem over2 init fail with code %d", sem_status);
        exit(0);
    }

    msgqid = msgget(IPC_PRIVATE,0666|IPC_CREAT); // 得到消息队列标识符或创建一个消息队列对象
                                                 // 0666 任何人可写
    printf("msgqid: %d\n", msgqid);
    if(msgqid<0)
    {
        printf("msgget: failed!\n");
        exit(1);
    }


    pthread_create(&sender1d,NULL,sender1,NULL); // 启动sender1线程
    pthread_create(&sender2d,NULL,sender2,NULL); // 启动sender2线程
    pthread_create(&received,NULL,receive,NULL); // 启动receiver线程

    pthread_join(sender1d,NULL); // 等待线程结束
    pthread_join(sender2d,NULL); // 等待线程结束
    pthread_join(received,NULL); // 等待线程结束

    return 0;
}