CC= gcc
OUTPUT= main
LD= gcc
SENDER_O= sender.o
RECEIVER_O= receiver.o

build_main: build_sender build_receiver
	$(CC) -o $(OUTPUT) main.c $(SENDER_O) $(RECEIVER_O) 

build_sender:
	$(CC) -o $(SENDER_O)  -c sender/sender.c 

build_receiver:
	$(CC) -o $(RECEIVER_O)  -c receiver/receiver.c 

clean:
	rm $(OUTPUT) $(RECEIVER_O) $(SENDER_O)