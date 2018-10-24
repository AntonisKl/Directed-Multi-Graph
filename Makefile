CC = gcc
CFLAGS  = -g -Wall


default: mygraph

mygraph:  mygraph.o graph.o utils.o stack.o 
	$(CC) $(CFLAGS) -o mygraph mygraph.o graph.o utils.o stack.o 

mygraph.o:  mygraph.c
	$(CC) $(CFLAGS) -c mygraph.c

graph.o:  graph.c
	$(CC) $(CFLAGS) -c graph.c

utils.o:  utils.c
	$(CC) $(CFLAGS) -c utils.c

stack.o:  stack.c
	$(CC) $(CFLAGS) -c stack.c


clean: 
	$(RM) mygraph *.o