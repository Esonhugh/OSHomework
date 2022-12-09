#include<unistd.h>
#include "debug.h"

typedef int pid_t;

// debugging.
int print_args(int argc, char ** agrv) {
	#ifdef DEBUG_MODE
	printf("Calling: args[%d]: \n", argc);
	for (int i = 0; i <= argc ; i++) {
		printf("Args[%d]: %s\n", i,agrv[i]);
	}
	return 0;
	#endif
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
		printf("remote process:%d exit code:%d\n", pid, exitCode);
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

		//let kernel take control
		if (argc != 0) {
			execvp(path, argv);
		} else {
			execl(path, "");
		}
		exit(0);
	}
	return 0;
}