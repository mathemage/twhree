CC = gcc
CFLAGS = -g -O1
OBJECTS = main.o twhree.o

twhree : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o twhree

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(OBJECTS)
