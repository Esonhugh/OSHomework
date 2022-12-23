#ifndef MSGS

#define MSGS
#define Max_Message_Size 512
// 消息类型
#define Send2Recv 1
#define Recv2Send 2
// 消息方向
#define SEND1_TO_RECV 1
#define SEND2_TO_RECV 2
#define RECV_TO_SEND1 3
#define RECV_TO_SEND2 4

// Global Messages defines.
typedef struct MSG
{
    long mtype; // 第一个字段必须为long类型，即表明此发送消息的类型，msgrcv根据此接收消息
    char mtext[Max_Message_Size]; // 消息正文，可以是任何类型
    int mdir; // 用于给接收线程区分消息是谁发送的
} message;

#endif
