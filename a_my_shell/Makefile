CC = aarch64-linux-gnu-gcc

main: 
	$(CC) -o main main.c pipe_system.h pipe_system.c

pipe_system:
	$(CC) -o pipe_system.so -fPIC -c pipe_system.c

clean: 
	rm main