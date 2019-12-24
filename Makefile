CC = cc
CFLAGS =-Wall -g -lm

EXECS = \
	maintest

all : $(EXECS)

clean:
	rm -rf $(EXECS) *.o

maintest: brute_force.o basic_Q.o TSP_parser_Q.o arg_parser.o main.o
	$(CC) -o $@ $^ $(CFLAGS)


%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
