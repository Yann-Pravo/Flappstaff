CC = gcc
CFLAGS = -Wall
LDFLAGS = -lcurl

OBJS = ./src/source/FacebookLib.o ./src/source/TPersonne.o ./src/source/Graphe.o ./src/source/Flapstaff.o ./src/source/ParseFile.c ./src/source/ExportGraph.c

all: flapstaff

flapstaff: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o ./src/source/*.o *.gcda *.gcno ./src/source/*.gcda ./src/source/*.gcno

superclean: all clean