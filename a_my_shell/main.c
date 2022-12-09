#define DEBUG_MODE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pipe_system.h"
#include "debug.h"

// process_input return status Code
int process_input(char **ret_argv) {
    // get command name and args
    // 定义字符串数组
    // 定义 argc 和 argv
    int argc = -1;
    char **argv = NULL;
    // 动态分配内存
    argv = (char **)malloc(1 * sizeof(char *)); //64
    printf("Shell Promot> ");

    char cmd[MAX_LINE_LEN];
    fflush(stdout);
    // 读取用户输入的命令
    if (fgets(cmd, MAX_LINE_LEN, stdin) == NULL) {
        return argc;
    }
    DEBUG_STRING("cmd",cmd);
    // 删除换行符
    cmd[strlen(cmd) - 1] = '\0';
    DEBUG_INT("cmdlen", strlen(cmd));
    char *command_current = cmd;
    command_current = strtok(command_current, " ");
    while(command_current != NULL){
        argc++; // update new argv
        DEBUG_INT("argc", argc);
        DEBUG_STRING("current command",command_current);
        char empty;
            // argc 为 0
            argv[argc] = (char *)malloc((strlen(command_current) + 1) * sizeof(char)); // extend argv
            strcpy(argv[argc], command_current); // Save String;
            argv = (char **)realloc(argv, (argc+1)* sizeof(char *));  // extend a buffer.
        // print_args(argc, argv);
        command_current = strtok(NULL, " ");
    }
    printf("\n");
    ret_argv = argv;
    print_args(argc, ret_argv);
    return argc;
}


int main(int argc, char *argv[]) 
{
    while (1) {
    char **strings = NULL;
    int argc = process_input(strings);
    if(argc == EOF) {
        printf("Empty Command");
        break;
    }
        DEBUG_STRING("strings[0]", strings[0]);
        int ret = piped_system(strings[0], argc, strings );
        printf("pipe_system returns %d", ret);
    }
}