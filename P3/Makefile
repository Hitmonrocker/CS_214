CCFLAGS = -ggdb -Wall

all: sc 

sc: sorter_server.c is.c
	gcc $(CCFLAGS) -lpthread -o sc sorter_server.c sorter_server.h

clean:
	rm -rf sc

