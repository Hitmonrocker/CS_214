CCFLAGS = -ggdb -Wall

all: threads 

threads: threads.c threads.h is.c sort_starter.c sorterQuickStruct.c
	gcc $(CCFLAGS) -pthread -o threads threads.c threads.h

clean:
	rm -rf threads

