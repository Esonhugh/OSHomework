#ifndef PIPE_SYSTEM
#define PIPE_SYSTEM
int piped_system(const char *path, int argc, char **argv);
int print_args(int argc, char ** agrv);
#endif