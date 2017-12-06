CCFLAGS = -ggdb -Wall

all: sc 

sc: sorter_client.c is.c
	gcc $(CCFLAGS) -pthread -o sc sorter_client.c sorter_client.h

clean:
	rm -rf sc

