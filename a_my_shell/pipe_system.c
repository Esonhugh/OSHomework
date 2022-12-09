#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "debug.h"

typedef int pid_t;


#ifndef PRINT_ARGS
#define PRINT_ARGS
// debugging.
int print_args(int argc, char ** argv) {
#ifdef DEBUG_MODE
    printf("Calling: args[%d]: \n", argc);
    for (int i = 0; i <= argc ; i++) {
        printf("Args[%d]: %s\n", i, argv[i]);
    }
#endif
    return 0;
}
#endif

// process_input return status Code
int process_input(char ***ret_argv) {
    // get command name and args
    // 定义字符串数组
    // 定义 argc 和 argv
    int argc = -1;
    char **argv = NULL;
    // 动态分配内存
    argv = (char **)malloc(1 * sizeof(char *));
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
        argv[argc] = (char *)malloc((strlen(command_current) + 1) * sizeof(char));  // extend argv
        strcpy(argv[argc], command_current);    // Save String;
        argv = (char **)realloc(argv, (argc+1)* sizeof(char *));  // extend a buffer.
        // print_args(argc, argv);
        command_current = strtok(NULL, " ");
    }
    printf("\n");
    *ret_argv=argv;
    //memcpy(p_ret_argv,&argv,sizeof(void*));
    print_args(argc, *ret_argv);
    return argc;
}




// pipe system
int piped_system(const char *path, int argc, char **argv) 
{
	DEBUG("pipe system called");
    // Create Stdin Pipe
	int stdinPair[2];
	if (pipe(stdinPair) < 0) {
		return -1;
	}
	int readStdinFD = stdinPair[0];
	int writeStdinFD = stdinPair[1];

	// Create Stdout Pipe
    int stdoutPair[2];
	if (pipe(stdoutPair) < 0) {
		return -1;
	}
	int readStdoutFD = stdoutPair[0];
	int writeStdoutFD = stdoutPair[1];

    // Create Subprocess.
	pid_t pid = fork();
	if (pid < 0) {
		// Forked Error
		return -1;
	}
	if (pid > 0) { //parent Program
        // Closed pipes
		close(readStdinFD);
		close(writeStdoutFD);
		char buf[1024] = { 0 };
		//redirect stdin to our Stdin 
		dup2(STDIN_FILENO, writeStdinFD);
		while (1) {
			size_t readCount = read(readStdoutFD, buf, 1024);
			if (readCount <= 0) {
				break; // read Error
			}
			printf("child stdout:%s", buf);
			memset(buf, 0, 1024);
		}
        // Close when Exit.
		close(writeStdinFD);
		close(readStdoutFD);
		printf("parent:%d wait remote process:%d to exit\n", getpid(), pid);
		int exitCode = 0;
		waitpid(pid, &exitCode, 0);
	} else { 
        // child Program
		close(writeStdinFD);
		close(readStdoutFD);
		// redirect pipe read to write to stdin
		dup2(STDIN_FILENO, readStdinFD);
		dup2(STDOUT_FILENO, writeStdoutFD);
		const char *writebuf = "child process prepare to execve\n";
		printf("child process output\n");
		printf("child received args:");
		print_args(argc, argv);
        printf("\n");

		//let kernel take control
		if (argc != 0) {
			return execvp(path, argv);
		} else {
			return execl(path, "");
		}
	}
	return 0;
}