CCFLAGS = -ggdb -Wall

all: threads 

threads: threads.c threads.h
	gcc $(CCFLAGS) -pthread -o threads threads.c threads.h

clean:
	rm -rf threads

