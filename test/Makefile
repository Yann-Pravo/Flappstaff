CC ?= gcc
CFLAGS = -Wall -fprofile-arcs -ftest-coverage
LDFLAGS = -Lcu/ -lcu -lcurl

OBJS = testParseFile.o testGraphe.o testExportGraph.o testFacebookLib.o testPersonne.o testsRunner.o ../src/source/ExportGraph.o ../src/source/FacebookLib.o ../src/source/TPersonne.o ../src/source/ParseFile.o ../src/source/Graphe.o

all: testsRunner

testsRunner: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o ../src/source/*.o

.PHONY: clean all clean
