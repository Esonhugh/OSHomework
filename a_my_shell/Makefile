CC = gcc
mainfile = main.c
pipesysfile = pipe_system.c pipe_system.h


debug:
	$(CC) -o main -D DEBUG_MODE  $(pipesysfile) $(mainfile)

build:
	$(CC) -o main  $(pipesysfile) $(mainfile)

pipe_system:
	$(CC) -o pipe_system.so -fPIC -c $(pipesysfile)

clean: 
	rm main
