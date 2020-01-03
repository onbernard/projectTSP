CC = cc
CFLAGS =-Wall -g -lm

EXECS = \
	maintest

all : $(EXECS)

clean:
	rm -rf $(EXECS) *.o

maintest: genetic_algorithm.o two_opt.o random_walk.o nearest_neighbour.o brute_force.o basic_Q.o TSP_parser_Q.o arg_parser.o main.o
	$(CC) -o $@ $^ $(CFLAGS)


%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
