#ifndef __MEM_SHARE
#define __MEM_SHARE
#include <semaphore.h>

#define MEM_MIN_SIZE 1024 // 需要共享的内存容量
#define KEY 8             // 共享内存段命名

extern sem_t *sem_sender;     // 可写入共享内存的信号量
extern sem_t *sem_receiver1;  // receiver 可读 信号量
extern sem_t *sem_receiver2;  // sender 可读 信号量


extern int shmid;  // 共享内存标识符
extern void *shmp; // 共享内存映射到新虚拟地址空间的起始地址

extern void init_signal(); // init signals
extern void clean_signal(); // clean signals

#endif
