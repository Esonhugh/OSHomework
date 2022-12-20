CC = gcc
mainfile = main.c
pipesysfile = pipe_system.c pipe_system.h


bpf_prog_syscall = 'tracepoint:syscalls:sys_enter_execve { printf("bpf syscall trace: %s process Calls program %s,\n", comm, str(args->filename)) }'
bpf_prog_uprobe_piped_system  = 'uprobe:./main:piped_system { printf("bpf uprobe trace: %s calling pipe_system with bin/script %s\n", comm, str(arg0) ) }'

ebpf:
ifdef syscall
	bpftrace -e $(bpf_prog_syscall) |grep main
endif
ifdef func
	bpftrace -e 'uprobe:./main:$(func) { printf("bpf uprobe trace: %s calling $(func)\n => frist arg: %s\n", comm, str(arg0) ) }'
else 
	bpftrace -e $(bpf_prog_uprobe_piped_system)
endif

debug:
	$(CC) -o main -D DEBUG_MODE  $(pipesysfile) $(mainfile)

build:
	$(CC) -o main  $(pipesysfile) $(mainfile)

pipe_system:
	$(CC) -o pipe_system.so -fPIC -c $(pipesysfile)

clean: 
	rm main
