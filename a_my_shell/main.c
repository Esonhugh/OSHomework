#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pipe_system.h"
#include "debug.h"

int main(int argc, char *argv[]) {
    while (1) {
        char **strings = NULL;
        int argc = process_input(&strings);
        if(argc == EOF) {
            printf("Empty Command");
            break;
        }
        DEBUG_STRING("strings[0]", strings[0]);
        if (strcmp(strings[0], "exit")==0) {
            printf("GoodBye!\n");
            exit(0);
        }
        int ret = piped_system(strings[0], argc, strings );
        DEBUG_INT("pipe_system return code:", ret);
    }
}