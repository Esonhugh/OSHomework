#ifndef __SEM
#define __SEM

#include <semaphore.h>
/*
  send:   sender线程向receive发消息的信号量,用于sender1和sender2之间的互斥。
  recv:   receive接受sender线程发送消息的信号量，提示receive什么时候可以取信息了。
  over1:  当receive接受到sender1发送的end1时，释放over1以提示结束线程sender1.
  over2:  当receive接受到sender2发送的end2时，释放over1以提示结束线程sender2.
*/
extern sem_t send, recv, over1, over2;

// message id to identify  in message queue.
extern int msgqid;

extern int over_status1, over_status2;

#endif
