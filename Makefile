CC=gcc
CFLAGS=-O2 -Wall

all: project

project: project.c database.o utils.o sql.o check.o table.o
	$(CC) $(CFLAGS) -o project $^

%o: %c %h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o project
