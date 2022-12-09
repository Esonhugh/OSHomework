CC = gcc
mainfile = main.c
pipesysfile = pipedream.c pipedream.h


debug:
	$(CC) -o main -D DEBUG_MODE -c $(mainfile) $(pipesysfile)

build:
	$(CC) -o main -c $(mainfile) $(pipesysfile)

pipe_system:
	$(CC) -o pipe_system.so -fPIC -c $(pipesysfile)

clean: 
	rm main
