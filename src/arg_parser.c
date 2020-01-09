#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arg_parser.h"

#include "globals.h"

/**
 * \file    arg_parser.c
 * \brief   SOURCE - Parses the arguments given to main, entry point is parseArguments, returns a struct args_t that contains the informations
 * \author  BERNARD O.
 * \date    december 2019
 */


///
/// THIS IS THE ENTRY POINT TO THE ARGUMENT PARSER.
/// IT ITERATES OVER ALL ARGUMENTS, FILLING THE BUFFER AS IT DOES AND STOPPING
/// WHEN IT ENCOUNTERS A FORMATTING ERROR.
///
/// FOR GA, ZERO, ONE, TWO, OR THREE FOLLOWING NUMERICAL ARGUMENTS ARE ALLOWED
///
/// IF -V FLAG IS GIVEN, global variable verbose is set to 1
///
/// LEAVES TOURFILENAME, LOGFILENAME AND OUTPUTFILENAME UNCHANGED WHEN UNSPECIFIED
///
int parseArguments(int argc, char **argv, args_t *argsBuffer){
    _Bool f_FlagDeclared = 0; // TO KNOW IF TWO FLAG ARE GIVEN
    _Bool t_FlagDeclared = 0; // IDEM
    _Bool o_FlagDeclared = 0; // IDEM
    _Bool v_FlagDeclared = 0; // IDEM
    int returnValue = 1;

    int i = 1;
    while(i < argc){ // ITERATE OVER ARGV
        // ========================= HELP =======================
        if( strcmp(argv[i], "-h") == 0 ){
            returnValue = 0;
            ++i;
        }
        // ========================= TSP FILE =======================
        else if( strcmp(argv[i], "-f") == 0 ){
            if(f_FlagDeclared){
                fprintf(stderr, "ARGUMENT ERROR : -f flag can only be declared once\n");
                return -1;
            }
            else if(i > argc-2){
                fprintf(stderr, "ARGUMENT ERROR : -f flag expects a following file name, but no arguments left\n");
                return -1;
            }
            else if( isReservedFlag(argv[i+1]) ){
                fprintf(stderr, "ARGUMENT ERROR : -f flag expects a following file name, but another flag is next to it\n");
                return -1;
            }
            strncpy(argsBuffer->TSPfileName, argv[i+1], MAXNAMELENGTH);
            f_FlagDeclared = 1;
            i += 2;
        }
        // ========================= TOUR FILE =======================
        else if( strcmp(argv[i], "-t") == 0 ){
            if(t_FlagDeclared){
                fprintf(stderr, "ARGUMENT ERROR : -t flag can only be declared once\n");
                return -1;
            }
            else if(i > argc-2){
                fprintf(stderr, "ARGUMENT ERROR : -t flag expects a following file name, but no arguments left\n");
                return -1;
            }
            else if( isReservedFlag(argv[i+1]) ){
                fprintf(stderr, "ARGUMENT ERROR : -t flag expects a following file name, but another flag is next to it\n");
                return -1;
            }
            strncpy(argsBuffer->TOURfileName, argv[i+1], MAXNAMELENGTH);
            t_FlagDeclared = 1;
            i += 2;
        }
        // ========================= OUTPUT FILE =======================
        else if( strcmp(argv[i], "-o") == 0 ){
            if(o_FlagDeclared){
                fprintf(stderr, "ARGUMENT ERROR : -o flag can only be declared once\n");
                return -1;
            }
            else if(i > argc-2){
                fprintf(stderr, "ARGUMENT ERROR : -o flag expects a following file name, but no arguments left\n");
                return -1;
            }
            else if( isReservedFlag(argv[i+1]) ){
                fprintf(stderr, "ARGUMENT ERROR : -o flag expects a following file name, but another flag is next to it\n");
                return -1;
            }
            strncpy(argsBuffer->outputfileName, argv[i+1], MAXNAMELENGTH);
            o_FlagDeclared = 1;
            i += 2;
        }
        // ========================= VERBOSE MODE =======================
        else if( strcmp(argv[i], "-v") == 0 ){
            if(v_FlagDeclared){
                fprintf(stderr, "ARGUMENT ERROR : -v flag can only be declared once\n");
                return -1;
            }
            verbose = 1;
            v_FlagDeclared = 1;
            ++i;
            if( i<argc && !isReservedFlag(argv[i]) ){
                strncpy(argsBuffer->logfileName, argv[i], MAXNAMELENGTH);
                ++i;
            }
        }
        // ========================= SOLVING METHODS =======================
        else if( strcmp(argv[i], "-bf") == 0 ){
            argsBuffer->solvingMethods += BF;
            ++i;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-bfm") == 0 ){
            argsBuffer->solvingMethods += BFM;
            ++i;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-ppv") == 0 ){
            argsBuffer->solvingMethods += PPV;
            ++i;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-rw") == 0 ){
            argsBuffer->solvingMethods += RW;
            ++i;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-2opt") == 0 ){
            argsBuffer->solvingMethods += TWOOPT;
            ++i;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-ga") == 0 ){
            argsBuffer->solvingMethods += GA;
            ++i;
            if( i<argc && atoi( argv[i])>0 ){
                argsBuffer->ga_nSpecimens = atoi(argv[i]);
                ++i;
                if( i<argc && atoi( argv[i])>0 ){
                    argsBuffer->ga_nGenerations = atoi(argv[i]);
                    ++i;
                    if( i<argc && atof( argv[i])>0.0 ){
                        argsBuffer->ga_mutationRate = atof(argv[i]);
                        ++i;
                    }
                }
            }
        }
        else if( strcmp(argv[i], "-nz") == 0 ){
            argsBuffer->nz = 1;
            i++;
        }
        // ----------------------------------------------
        else {
            fprintf(stderr, "ARGUMENT ERROR : unexpected argument \" %s \"\n", argv[i]);
            return -1;
        }
    }

    if(!f_FlagDeclared){
        fprintf(stderr, "ARGUMENT ERROR : a tsp file is required\n");
        return -1;
    }
    if( argsBuffer->solvingMethods == 0 ){
        fprintf(stderr, "ARGUMENT ERROR : at least one solving method is required\n");
        return -1;
    }
    return returnValue;
}

///
/// USED IN MAIN, NOT IN THIS FILE
/// Set strings and solving methods to 0 on every byte
/// Set parameters for the genetic algorithm to be 20 specimens, 200 generations
/// and a mutation rate of .3
///
void init_args_t(args_t *args){
    memset(args->TSPfileName, 0, MAXNAMELENGTH * sizeof(char));
    memset(args->TOURfileName, 0, MAXNAMELENGTH * sizeof(char));
    memset(args->logfileName, 0, MAXNAMELENGTH * sizeof(char));
    memset(args->outputfileName, 0, MAXNAMELENGTH * sizeof(char));
    args->solvingMethods = 0b0;
    args->ga_nSpecimens = 20;
    args->ga_nGenerations = 200;
    args->ga_mutationRate = 0.3;
    args->nz = 0;
}

///
/// FOR DEBUGGING AND VERBOSE MODE
///
void print_args(FILE *fp, args_t args){
    fprintf(fp, "TSP file name: %s\nTOUR file name: %s\nlog file name: %s\noutput file name: %s\n", args.TSPfileName, args.TOURfileName, args.logfileName, args.outputfileName);
    fprintf(fp, "verbose mode: %d\n", verbose);
    fprintf(fp, "NZ mode: %d\n", args.nz);
    fprintf(fp, "BF: %d\n", (args.solvingMethods & BF) != 0);
    fprintf(fp, "BFM: %d\n", (args.solvingMethods & BFM) != 0 );
    fprintf(fp, "PPV: %d\n", (args.solvingMethods & PPV) != 0 );
    fprintf(fp, "RW: %d\n", (args.solvingMethods & RW) != 0 );
    fprintf(fp, "2OPT: %d\n", (args.solvingMethods & TWOOPT) !=0 );
    fprintf(fp, "GA: %d\n", (args.solvingMethods & GA) != 0 );
    fprintf(fp, "    ga, number of specimens: %d\n", args.ga_nSpecimens);
    fprintf(fp, "    ga, number of generations: %d\n", args.ga_nGenerations);
    fprintf(fp, "    ga, mutation rate: %lf\n", args.ga_mutationRate);
}

///
/// USED IN PARSEARGUMENTS
///
_Bool isReservedFlag(char *str){
    return !( strcmp(str, "-f") && strcmp(str, "-t") && strcmp(str, "-v") && strcmp(str, "-o") && strcmp(str, "-h") && strcmp(str, "-nz") && strcmp(str, "-bf") && strcmp(str, "-bfm") && strcmp(str, "-ppv") && strcmp(str, "-rw") && strcmp(str, "-2opt") && strcmp(str, "-ga") );
}

