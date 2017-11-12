CCFLAGS = -ggdb -Wall

all: threads 

threads: threads.c 
	gcc $(CCFLAGS) -pthread -o threads threads.c

clean:
	rm -rf threads

