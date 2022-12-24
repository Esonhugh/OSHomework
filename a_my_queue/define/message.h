#ifndef MSGS
#define MSGS

#define Max_Message_Size 512
// message type
#define Send2Recv 1
#define Recv2Send 2
// message directions
#define SEND1_TO_RECV 1
#define SEND2_TO_RECV 2
#define RECV_TO_SEND1 3
#define RECV_TO_SEND2 4

// Global Messages defines.
typedef struct MSG
{
    // Message type
    long mtype;                   // (long type)the message type
    char mtext[Max_Message_Size]; // message data
    int mdir;                     // whois the sender?
} message;                        // alias as message

#endif
