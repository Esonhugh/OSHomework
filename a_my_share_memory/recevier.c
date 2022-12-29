#include "memory_share/memory_share.h"
#include <string.h>
#include <stdio.h>

int main() {
    pid_t pid = getpid(); 
    printf("recevier process launched as PID: %d\n", pid);
    
    init_signal();

    char temp[MEM_MIN_SIZE], r_str[100], *p;
    sem_wait(sem_receiver1); 
    // Block until sender complete

    strcpy(r_str, (char *)shmp); 
    p = strchr(r_str,':'); // 解构
    *(p++) = '\0'; // add last char == 0x00

    printf("recevied from %s: %s\n", r_str,p);
    
    sem_wait(sem_sender); // until i Can write into memory
    
    sprintf(temp,"%d:%s", pid, "over"); // struct message
    strcpy((char *)shmp, temp); 
    
    sem_post(sem_receiver2); // notify sender complete
    return 0;
}