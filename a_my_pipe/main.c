#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define BUF_MAX_SIZE 8192

    int pipefd[2]; //记录管道文件描述符，fdpipefd[0] → r； pipefd[1] → w
    int pid[3]; // 记录三个子进程的pid
    long n; //记录统计数据结果的变量
    char buf[BUF_MAX_SIZE]; // 测试阻塞用于写入管道的东西
    char str[1]; // 读管道字符串存储
    
    sem_t write_mutex; // 读锁
    sem_t read_mutex; // 写锁

void process_1() {
        sem_wait(&write_mutex); // 上写锁 此时write_mutex=0
        sem_wait(&read_mutex); // 上读锁 read_mutex=0

        long count = 0;
        close(pipefd[0]); // 关闭子进程管道读端

        // 管道默认是阻塞写，通过`fcntl`设置成非阻塞写，在管道满无法继续写入时返回 -1，作为循环终止条件
        fcntl(pipefd[1], F_SETFL, fcntl(pipefd[1], F_GETFL) | O_NONBLOCK);

        // 循环写入管道，直到返回值为 -1
        do {
            n = write(pipefd[1], buf, BUF_MAX_SIZE);
            if (n>=0) count+=n;
            printf("  thread 1 write %zdB\n", n);
        } while (n != -1);
        printf("  default pipeline = %ldKB\n", count / 1024); // 计算出写了多少

        sem_post(&read_mutex); // 释放写锁 此时write_mutex=1
        sem_post(&write_mutex); // 释放读锁 read_mutex=1
        exit(0);
}

typedef void func();

void CreatePipeAndFork(int pid_a,int *pipefd[2], func subprocess) {
    if (pipe(pipefd) < 0) {
        perror("pipe init error");
        exit(-1);
    }
    // 创建第一个子进程并检查操作是否成功
    pid[pid_a] = fork();
    if (pid[pid_a] < 0) {
        perror("fork error");
        exit(-1);
    }
    // 第一个子进程，利用非阻塞写测试管道大小
    if (pid[pid_a] == 0) {
        subprocess();
    }
}

int main() {
    printf("Init Prog\n");
    // 创建无名信号量
    sem_init(&write_mutex, 1, 1); //第二个参数 等于 0，信号量在线程间共享（常用）；不等于0，信号量在进程间共享
    sem_init(&read_mutex, 1, 1);  //第三个参数 信号量的初始值，由于一次只能有一个进程访问，所以这里设为1


    //  测试管道大小
    memset(buf, 0, BUF_MAX_SIZE); // 可有可无，将要写入管道的数据全设置成0
    // 创建管道并检查操作是否成功
    /*
    if (pipe(pipefd) < 0) {
        perror("pipe init error");
        exit(-1);
    }
    // 创建第一个子进程并检查操作是否成功
    pid[0] = fork();
    if (pid[0] < 0) {
        perror("fork error");
        exit(-1);
    }
    // 第一个子进程，利用非阻塞写测试管道大小
    if (pid[0] == 0) {
        process_1();
    } */
    CreatePipeAndFork(0, &pipefd, process_1);

    // 阻塞写完 一口气读完
    waitpid(pid[0], NULL, 0); //等待子进程返回
    close(pipefd[1]); //关闭父进程管道写端

    sem_wait(&read_mutex); // 上读锁 read_mutex=0
    // 设置非阻塞性读，作为循环结束标志
    fcntl(pipefd[0], F_SETFL, fcntl(pipefd[0], F_GETFL) | O_NONBLOCK);
    long count = 0;
    do {
        n = read(pipefd[0], str, 1);
        if (n>0) {
            count += n;
        }
    } while (n > 0);
    printf("main process %zdB read from process 1\n", count); // 顺利读到64KB数据，管道被清空
    sem_post(&read_mutex); // 释放读锁 read_mutex=1
    close(pipefd[0]); // 关闭父进程读端


    //  测试并发控制
    // 重新创建一个管道，获取新的描述符
    if (pipe(pipefd) < 0) {
        perror("pipe init error");
        exit(-1);
    }
    // 创建第二个子进程并检查操作是否成功
    pid[1] = fork();
    if (pid[1] < 0) {
        perror("fork error");
        exit(-1);
    }
    if (pid[1] == 0) {
        sem_wait(&write_mutex); // 上写锁
        close(pipefd[0]);

        n = write(pipefd[1], "[msg]process 2 writing...\n", 26); // 阻塞写入26字节数据
        printf("  process 2 written %zdB\n", n);

        sem_post(&write_mutex); // 释放写锁
        exit(0);
    }

    // 创建第三个子进程并检查操作是否成功
    pid[2] = fork();
    if (pid[2] < 0) {
        perror("fork error");
        exit(-1);
    }
    if (pid[2] == 0) {
        sem_wait(&write_mutex); // 上写锁
        close(pipefd[0]);

        n = write(pipefd[1], "[msg]process 3 writing...\n", 26); // 阻塞写入26字节数据
        printf("  process 3 written %zdB\n", n);

        sem_post(&write_mutex); // 释放写锁
        exit(0);
    }
    close(pipefd[1]);

    // 阻塞读数据
    int waiting = -2; // 需要读到两个进程的全部数据才能结束读
    do {
        n = read(pipefd[0], str, 1);
        printf("%c", str[0]);
        if (str[0] == '\n') waiting++; // 读到约定的结束符号即认定该进程的数据已经读完
    } while (n!=-1 && waiting < 0);
    close(pipefd[0]);


//  阻塞 无写入端引用读

    if (pipe(pipefd) < 0) { // 创建的管道读端写端都是此进程
        perror("pipe init error");
        exit(-1);
    }
    close(pipefd[1]); // 关掉写端，管道的写端引用为0
    n = read(pipefd[0], str, 1); // 尝试读数据，直接返回0，相当于读完了
    printf("【阻塞 无写入端引用读】接收到返回值：%ld\n", n);
    close(pipefd[0]);

//  阻塞 无读端写

    signal(SIGPIPE, SIG_IGN); // 忽略pipe异常，防止进程退出

    if (pipe(pipefd) < 0) {
        perror("pipe init error");
        exit(-1);
    }
    close(pipefd[0]); // 关掉读端，管道的读端引用为0
    n = write(pipefd[1], "666\n", 4); // 尝试写数据，返回异常
    printf("【阻塞 无读端写】接收到返回值：%ld", n);
    close(pipefd[1]);

    sem_destroy(&write_mutex); // 删除写锁信号量
    sem_destroy(&read_mutex); // 删除读锁信号量
    return 0;
}