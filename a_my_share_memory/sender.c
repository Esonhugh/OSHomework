#include "memory_share/memory_share.h"
#include <stdio.h>
#include <string.h>

int main() {
    pid_t pid = getpid(); // 
    printf("Sender Process Launched as PID: %d\n", pid);
    init_signal();

    char temp[MEM_MIN_SIZE], s_str[100], r_str[100], *p;
    printf("发送：");
    fflush(stdout);
    scanf("%[^\n]%*c",s_str); // Get Input
    
    sem_wait(sem_sender); // Block until I can write
    sprintf(temp,"%d:%s", pid, s_str); 
    strcpy((char *)shmp, temp); // 

    sem_post(sem_receiver1); // Notify Recevier to read
    sem_post(sem_sender); // free Memory Still can writed
    printf("POST SUCCESS!\n"); 

    sem_wait(sem_receiver2); // until recevier complete and  call back
    strcpy(r_str, (char *)shmp); 
    p = strchr(r_str,':'); // Destruct
    *(p++) = '\0';
    printf("Recevie Process %s Message: %s\n", r_str,p);
    clean_signal(); // Clean up
    return 0;
}