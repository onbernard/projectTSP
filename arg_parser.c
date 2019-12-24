#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arg_parser.h"

#include "globals.h"


void init_args_t(args_t *a){
    a->TSPfileName[0] = '\0';
    a->TOURfileName[0] = '\0';
    a->logfileName[0] = '\0';
    a->solvingMethods = 0b0;
    a->ga_nSpecimens = 20;
    a->ga_nGenerations = 200;
    a->ga_mutationRate = 0.3;
}

void print_args(args_t a){
    printf("TSP file name: %s\nTOUR file name: %s\nlog file name: %s\n", a.TSPfileName, a.TOURfileName, a.logfileName);
    printf("verbose mode: %d\n", verbose);
    printf("NZ mode: %d\n", nz);
    printf("BF: %d\n", (a.solvingMethods & BF) != 0);
    printf("BFM: %d\n", (a.solvingMethods & BFM) != 0 );
    printf("PPV: %d\n", (a.solvingMethods & PPV) != 0 );
    printf("RW: %d\n", (a.solvingMethods & RW) != 0 );
    printf("2OPT: %d\n", (a.solvingMethods & TWOOPT) !=0 );
    printf("GA: %d\n", (a.solvingMethods & GA) != 0 );
    printf("    ga, number of specimens: %d\n", a.ga_nSpecimens);
    printf("    ga, number of generations: %d\n", a.ga_nGenerations);
    printf("    ga, mutation rate: %lf\n", a.ga_mutationRate);
}

// Return true if str is one of the flags
int isReservedFlag(char *str){
    return !( strcmp(str, "-f") && strcmp(str, "-t") && strcmp(str, "-v") && strcmp(str, "-o") && strcmp(str, "-h") && strcmp(str, "-nz") && strcmp(str, "-bf") && strcmp(str, "-bfm") && strcmp(str, "-ppv") && strcmp(str, "-rw") && strcmp(str, "-2opt") && strcmp(str, "-ga") );
}

// parseArguments   : parse the main arguments
//          IN      : argc, argv, a 32 long char buffer for TSP file and another one for TOUR file
//          OUT     : TSPFileNameBuffer will be changed, TOURFileNameBuffer will be set to the empty string if not provided
//          RETURN  : negative value if error, 0 help has to be displayed, positive if success
int parseArguments(int argc, char **argv, args_t *parsedArgs){
    _Bool f_FlagDeclared = 0;
    _Bool t_FlagDeclared = 0;
    int returnValue = 1;

    int i = 1;
    while(i < argc){
        // ----------------------------------------------
        if( strcmp(argv[i], "-h") == 0 ){
            returnValue = 0;
            ++i;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-f") == 0 ){
            if(f_FlagDeclared){
                fprintf(stderr, "ERROR : in parseArguments : argument error : -f flag can only be declared once\n");
                return -1;
            }
            else if(i > argc-2){
                fprintf(stderr, "ERROR : in parseArguments : argument error : -f flag expects a following file name, but no arguments left\n");
                return -1;
            }
            else if( isReservedFlag(argv[i+1]) ){
                fprintf(stderr, "ERROR : in parseArguments : argument error : -f flag expects a following file name, but another flag is next to it\n");
                return -1;
            }
            strncpy(parsedArgs->TSPfileName, argv[i+1], MAXNAMELENGTH);
            f_FlagDeclared = 1;
            i += 2;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-t") == 0 ){
            if(t_FlagDeclared){
                fprintf(stderr, "ERROR : in parseArguments : argument error : -t flag can only be declared once\n");
                return -1;
            }
            else if(i > argc-2){
                fprintf(stderr, "ERROR : in parseArguments : argument error : -t flag expects a following file name, but no arguments left\n");
                return -1;
            }
            else if( isReservedFlag(argv[i+1]) ){
                fprintf(stderr, "ERROR : in parseArguments : argument error : -t flag expects a following file name, but another flag is next to it\n");
                return -1;
            }
            strncpy(parsedArgs->TOURfileName, argv[i+1], MAXNAMELENGTH);
            t_FlagDeclared = 1;
            i += 2;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-v") == 0 ){
            verbose = 1;
            ++i;
            if( i<argc && !isReservedFlag(argv[i]) ){
                strncpy(parsedArgs->logfileName, argv[i], MAXNAMELENGTH);
                ++i;
            }
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-bf") == 0 ){
            parsedArgs->solvingMethods += BF;
            ++i;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-bfm") == 0 ){
            parsedArgs->solvingMethods += BFM;
            ++i;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-ppv") == 0 ){
            parsedArgs->solvingMethods += PPV;
            ++i;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-rw") == 0 ){
            parsedArgs->solvingMethods += RW;
            ++i;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-2opt") == 0 ){
            parsedArgs->solvingMethods += TWOOPT;
            ++i;
        }
        // ----------------------------------------------
        else if( strcmp(argv[i], "-ga") == 0 ){
            parsedArgs->solvingMethods += GA;
            ++i;
            if( i<argc && atoi(argv[i])>0 ){
                parsedArgs->ga_nSpecimens = atoi(argv[i]);
                ++i;
                if( i<argc && atoi(argv[i])>0 ){
                    parsedArgs->ga_nGenerations = atoi(argv[i]);
                    ++i;
                    if( i<argc && atof(argv[i])>0.0 ){
                        parsedArgs->ga_mutationRate = atof(argv[i]);
                        ++i;
                    }
                }
            }
        }
        else if( strcmp(argv[i], "-nz") == 0 ){
            nz = 1;
            i++;
        }
        // ----------------------------------------------
        else {
            fprintf(stderr, "ERROR : in parseArguments : argument error : unexpected argument \" %s \"\n", argv[i]);
            return -1;
        }
    }

    if(!f_FlagDeclared){
        fprintf(stderr, "ERROR : argument error : a tsp file is required\n");
        return -1;
    }
    if( parsedArgs->solvingMethods == 0 ){
        fprintf(stderr, "ERROR : argument error : at least one solving method is required\n");
        return -1;
    }
    return returnValue;
}