CC = gcc
mainfile = main.c
pipesysfile = pipedream.c pipedream.h


debug:
	$(CC) -o main -D DEBUG_MODE $(mainfile) $(pipesysfile) debug.h

build:
	$(CC) -o main $(mainfile) $(pipesysfile) debug.h

pipe_system:
	$(CC) -o pipe_system.so -fPIC -c $(pipesysfile)

clean: 
	rm main
