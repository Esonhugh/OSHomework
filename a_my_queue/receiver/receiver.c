#include "../define/message.h"
#include "../define/sem.h"
#include<sys/ipc.h>
#include<stdlib.h>

void* receive(void* arg)
{
    /*
        receive函数做的工作：
        1. 通过消息队列接受来自sender1和sender2两个线程发送的消息
        2. 将消息显示在屏幕上
        3. 收到end1时,向sender1发送over1.
        4. 受到end2时,向sender2发送over2.
        5. 消息全都接收完成后删除消息队列，结束所有线程的运行。
    */
    
    message recv_msg; // temply store current message
    while(1)
    {
        sem_wait(&recv); // 阻塞，直到可以接收消息，程序消息发送完成后会对此信号量进行 v 操作
        msgrcv(msgqid,&recv_msg,sizeof(message),Send2Recv, 0); // 接收消息
        int curDir = recv_msg.mdir;
        if(curDir==SEND1_TO_RECV) //sender1->receive
        {
            if(strcmp(recv_msg.mtext,"end1")==0)
            {
                // 如果是退出信号
                printf("Receiver get from sender1: %s\n",recv_msg.mtext);
                
                // 回复 "over1"
                strcpy(recv_msg.mtext,"over1"); // 消息赋值
                recv_msg.mdir = RECV_TO_SEND1; // 设置方向
                recv_msg.mtype = Recv2Send; // 设置消息类型
                msgsnd(msgqid,&recv_msg,sizeof(message), 0); // 发送回复消息

                over_status1 = 1; // sender1 结束
                sem_post(&over1); // over1信号量+1，sender1 可以继续处理接收回复
            }else{
                // 普通消息
                printf("Receiver get from sender1: %s\n",recv_msg.mtext); // 答疑
                sem_post(&send); // 允许继续发送消息
            }
        }
        else if(curDir==SEND2_TO_RECV) //sender2->receive
        {
            if(strcmp(recv_msg.mtext,"end2")==0)
            {
                printf("Receiver get from sender2: %s\n",recv_msg.mtext);

                strcpy(recv_msg.mtext,"over2");
                recv_msg.mdir = RECV_TO_SEND2;
                recv_msg.mtype = Recv2Send;
                msgsnd(msgqid,&recv_msg,sizeof(message), 0);

                over_status2 = 1; // sender2 结束
                sem_post(&over2); // over2信号量+1，sender2 可以继续处理接收回复
            }
            else
            {
                printf("Receiver get from sender2: %s\n",recv_msg.mtext); // 打印消息
                sem_post(&send); // 允许继续发送消息
            }
        }
        if(over_status1 && over_status2)
            break; //两个线程都发送了exit,那receiver再应答完over后就应该跳出while循环.
    }
    pthread_exit(NULL);
}