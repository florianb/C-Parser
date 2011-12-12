#CC = gcc
CC = clang
CCFLAGS = -Wall
#CCFLAGS = -fsyntax-only -std=c99
OBJ = parser.o amachine.o list.o
LDFLAGS = 

debug:
	make clean
	make parser && clear && valgrind ./parser

parser: $(OBJ)
	$(CC) $(CCFLAGS) -o parser $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f parser *.o