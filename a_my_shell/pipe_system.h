#ifndef PIPE_SYSTEM
#define PIPE_SYSTEM
int piped_system(const char *path, int argc, char **argv);
int process_input(char **ret_argv);
#endif