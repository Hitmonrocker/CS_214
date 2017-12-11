CCFLAGS = -ggdb -Wall

all: sc 

sc: sorter_server.c sorter.c
	gcc $(CCFLAGS) -lpthread -o sc sorter_server.c sorter_server.h sorter.c

clean:
	rm -rf sc

