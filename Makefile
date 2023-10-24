CC = gcc
CFLAGS = -Wall -Werror

all: pseudo-shell

pseudo-shell: main.o command.o string_parser.o
	$(CC) $(CFLAGS) -o pseudo-shell main.o command.o string_parser.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

command.o: command.c
	$(CC) $(CFLAGS) -c command.c

string_parser.o: string_parser.c
	$(CC) $(CFLAGS) -c string_parser.c

clean:
	rm -f *.o pseudo-shell