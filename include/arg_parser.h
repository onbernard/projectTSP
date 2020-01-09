#ifndef _CAPTURE_H
#define _CAPTURE_H

#include "globals.h"

/**
 * \file    arg_parser.h
 * \brief   HEADER - Parses the arguments given to main, entry point is parseArguments, returns a struct args_t that contains the informations
 * \author  BERNARD O.
 * \date    december 2019
 */

/// \def    Solving methods flags
#define BF 0b1
#define BFM 0b10
#define PPV 0b100
#define RW 0b1000
#define TWOOPT 0b10000
#define GA 0b100000

/** 
 * \struct  args_t arg_parser.h
 * \brief   Results of main arguments parsing
 * 
 * TSPfileName      :   -f <string>     TSP file name
 * TOURfileName     :   -t <string>     TOUR file name
 * logfileName      :   -v <string>     log file  name
 * outputfileName   :   -o <string>     csv output file
 * solvingMethods   :   holder fot bitwise flags defined in arg_parser.h
 * ga_nSpecimens    :   number of specimens for the ga method
 * ga_nGenerations  :   number of generations for the ga method
 * ga_murationRate  :   mutation rate for the ga method
 * nz               :   not zero : 0 if the point (0,0) is automaticly included, 0 if not
 */
typedef struct args_s{
    char TSPfileName[MAXNAMELENGTH];
    char TOURfileName[MAXNAMELENGTH];
    char logfileName[MAXNAMELENGTH];
    char outputfileName[MAXNAMELENGTH];
    unsigned char solvingMethods;
    unsigned int ga_nSpecimens;
    unsigned int ga_nGenerations;
    double ga_mutationRate;
    _Bool nz;
} args_t;

/**
 * \brief   Parses the main arguments, return the results in argsBuffer
 * 
 * \param   argc        :   same as in main
 * \param   argv        :   same as in main
 * \param   argsBuffer  :   buffer to where the results are written
 * 
 * \return  a negavive value if an error has occured, 0 if help has to be displayed, else a positive value
 */
int parseArguments(int argc, char **argv, args_t *argsBuffer);


/** 
 * \brief   Initialize an args_t structure
 * 
 *  \param  args : the args_t structure to be initialized
 */
void init_args_t(args_t *args);


/** \brief  Prints an args_t structure
 * 
 *  \param  fp   : A file pointer where args will be printed
 *  \param  args : the args_t structure to be printed
 */
void print_args(FILE *fp, args_t args);


/**
 * \brief   Check if str is -f/t/v/o/h/nz/bf/bfm/rw/2opt/ga
 * \param   str : The string to be checked
 * \return  1 if str is a reserved flag, 0 if not
 */
_Bool isReservedFlag(char *str);

#endif