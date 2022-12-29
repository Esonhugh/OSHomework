all: sender recevier

memory:
	gcc -o memory.o -c memory_share/memory_share.c

sender: memory
	gcc -o sender sender.c memory.o

recevier: memory
	gcc -o recevier recevier.c memory.o

clean: 
	rm recevier sender memory.o