#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // 字符串操作
#include <sys/shm.h>   // 共享内存
#include <unistd.h>    // 进程信息
#include <semaphore.h> // 信号量
#include <fcntl.h>     // control
#include <string.h>
#include "memory_share.h"

sem_t *sem_sender;     // 可写入共享内存的信号量
sem_t *sem_receiver1;  // receiver 可读 信号量
sem_t *sem_receiver2;  // sender 可读 信号量

int shmid;  // 共享内存标识符
void *shmp; // 共享内存映射到新虚拟地址空间的起始地址

void init_signal(); // init signals
void clean_signal(); // clean signals
void init_signal()
{
    //初始化信号量
    sem_sender = sem_open("sender", O_CREAT, 0666, 1); // 最初为空，可以写入，所以初始值为1
    sem_receiver1 = sem_open("receiver1", O_CREAT, 0666, 0);
    sem_receiver2 = sem_open("receiver2", O_CREAT, 0666, 0);

    //创建共享内存，key = 0(IPC_PRIVATE) 创建一个新对象。成功则返回id (一个与key相关的标识符)
    shmid = shmget(KEY, MEM_MIN_SIZE, 0666|IPC_CREAT);
    if(shmid < 0)
    {
        printf("创建共享内存出错！\n");
        exit(-1);
    }


    //指定共享内存映射到新虚拟地址空间，返回起始地址
    shmp = shmat(shmid, NULL, 0);
    if((long long int)shmp == -1)
    {
        printf("映射内存出错！\n");
        exit(-1);
    }
}

// clean_signal 清理信号量和共享内存
void clean_signal() {
    shmdt(shmp); // dt：detach 将共享内存从当前进程中分离
    shmctl(shmid, IPC_RMID, NULL); // ctl：control 删除共享内存
    sem_unlink("sender");    // 删除信号量
    sem_unlink("receiver1"); 
    sem_unlink("receiver2"); 
}