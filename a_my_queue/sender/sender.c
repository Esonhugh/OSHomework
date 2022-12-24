#include "../define/message.h"
#include "../define/sem.h"
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void *sender1(void *arg)
{
    /*
        sender1函数做的工作:
        在while循环里面
        {
            1. 接受用户输入的字符串
            2. 整合message,并将其加入到消息队列中
            3.  如果字符串是"exit",则发送end1.等待recv应答
        }
        4. 打印应答消息
        5. 结束进程
    */
    message send_msg; // 声明消息结构题
    while (1)
    {
        char inputStr[512];
        sem_wait(&send);                              // 阻塞直到可发送消息
        printf("Sender1:Please input your string: "); // 友好提示
        scanf("%[^\n]%*c", inputStr);                 // 读取整行
        send_msg.mdir = SEND1_TO_RECV;                // 标注方向，receiver 可以判断是从哪里来的消息
        send_msg.mtype = Send2Recv;                   // 设置消息类型，receiver 更具这个字段来接收消息

        // 先处理要退出的情况
        if (!strcmp(inputStr, "exit"))
        {
            // 如果是退出指令
            strcpy(send_msg.mtext, "end1");                // 消息结构体消息内容设置为"end1"
            msgsnd(msgqid, &send_msg, sizeof(message), 0); // 发送消息 第四个参数0：当消息队列满时，msgsnd将会阻塞，直到消息能写进消息队列
            sem_post(&recv);                               // 释放接收信号，让 receiver 线程能够处理消息
            break;
        }
        else
        {
            strcpy(send_msg.mtext, inputStr);              // 消息结构体消息内容设置为用户的输入
            msgsnd(msgqid, &send_msg, sizeof(message), 0); // 发送消息
            sem_post(&recv);                               // 释放接收信号
        }
        // 再次争夺发送信号量，直到输入"exit"
    }
    sem_wait(&over1); // 等待 receiver 确认返回
    // 接收 receiver 发来的 "over1" 消息
    msgrcv(msgqid, &send_msg, sizeof(message), Recv2Send, 0); // 从标识符为msqid的消息队列读取消息并存于msgp中，读取后把此消息从消息队列中删除
    printf("Sender1 has received: %s\n", send_msg.mtext);     // log
    printf("Thread1 has been shut down!\n");
    sem_post(&send);                  // 处理完毕，释放发送信号量，消息队列可以继续使用
    if (over_status1 && over_status2) // 不确定 sender1 和 sender2 哪个先结束，所以都写。如果全部 exit 了，需要删除消息队列
        msgctl(msgqid, IPC_RMID, 0);  // 删除消息队列
    pthread_exit(NULL);               // 线程退出
}

void *sender2(void *arg)
{
    /*
        sender2函数做的工作:
        在while循环里面
        {
            1. 接受用户输入的字符串
            2. 整合message,并将其加入到消息队列中
            3.  如果字符串是"exit",则发送end1.等待recv应答
        }
        4. 打印应答消息
        5. 结束进程
    */
    message send_msg; // 声明消息结构题
    while (1)
    {
        char inputStr[512];
        sem_wait(&send);                              // 阻塞直到可发送消息
        printf("Sender2:Please input your string: "); // 友好提示
        scanf("%[^\n]%*c", inputStr);                 // 读取整行
        send_msg.mdir = SEND2_TO_RECV;                // 标注方向，receiver 可以判断是从哪里来的消息
        send_msg.mtype = Send2Recv;                   // 设置消息类型，receiver 更具这个字段来接收消息

        // 先处理要退出的情况
        if (!strcmp(inputStr, "exit"))
        {
            // 如果是退出指令
            strcpy(send_msg.mtext, "end2");                // 消息结构体消息内容设置为"end1"
            msgsnd(msgqid, &send_msg, sizeof(message), 0); // 发送消息 第四个参数0：当消息队列满时，msgsnd将会阻塞，直到消息能写进消息队列
            sem_post(&recv);                               // 释放接收信号，让 receiver 线程能够处理消息
            break;
        }
        else
        {
            strcpy(send_msg.mtext, inputStr);              // 消息结构体消息内容设置为用户的输入
            msgsnd(msgqid, &send_msg, sizeof(message), 0); // 发送消息
            sem_post(&recv);                               // 释放接收信号
        }
        // 再次争夺发送信号量，直到输入"exit"
    }
    sem_wait(&over2); // 等待 receiver 确认返回
    // 接收 receiver 发来的 "over1" 消息
    msgrcv(msgqid, &send_msg, sizeof(message), Recv2Send, 0); // 从标识符为msqid的消息队列读取消息并存于msgp中，读取后把此消息从消息队列中删除
    printf("Sender2 has received: %s\n", send_msg.mtext);     // log
    printf("Thread2 has been shut down!\n");
    sem_post(&send);                  // 处理完毕，释放发送信号量，消息队列可以继续使用
    if (over_status1 && over_status2) // 不确定 sender1 和 sender2 哪个先结束，所以都写。如果全部 exit 了，需要删除消息队列
        msgctl(msgqid, IPC_RMID, 0);  // 删除消息队列
    pthread_exit(NULL);               // 线程退出
}
