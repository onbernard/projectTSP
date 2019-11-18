#ifndef _CAPTURE_H
#define _CAPTURE_H

#include "globals.h"

#define BF 0b1
#define BFM 0b10
#define PPV 0b100
#define RW 0b1000
#define TWOOPT 0b10000
#define GA 0b100000


typedef struct args{
    char TSPfileName[MAXNAMELENGTH];
    char TOURfileName[MAXNAMELENGTH];
    char logfileName[MAXNAMELENGTH];
    unsigned char solvingMethods;
    unsigned int ga_nSpecimens;
    unsigned int ga_nGenerations;
    double ga_mutationRate;
    _Bool nz;
} args_t;

void init_args_t(args_t *a);

void print_args_t(args_t a);

int isReservedFlag(char *str);

int parseArguments(int argc, char **argv, args_t *parsedArgs);

#endif