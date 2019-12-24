#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "res.h"
#include "TSP_parser_Q.h"
#include "arg_parser.h"
#include "brute_force.h"

/*
Usage :  ./tsp -f <file> [-t <tour>] [-v [<file>]] -<méthode> [-h]
-f <file> : nom du fichier tsp (obligatoire)
-t <file> : nom du fichier solution (optionnel)
-v [<file>] : mode verbose (optionnel), écrit dans un fichier si présent
-o <file> : export des résultats sans un fichier csv
-h : help, affiche ce texte

<méthode> : méthodes de calcul (cumulables) :

-bf : brute force,
-bfm : brute force en utilisant la matrice de distance,
-ppv : plus proche voisin,
-rw : random walk,
-2opt : 2 optimisation. Si -ppv ou -rw ne sont pas présentes on utilise -rw,
-ga <nombre d'individus> <nombre de générations> <taux de mutation> :
algorithme génétique, défaut = 20 individus, 200 générations, 0.3 mutation.
*/
/*
char *TSPfileName = NULL;
char *TOURfileName = NULL;
_Bool verbose = 0;
char *logFileName = NULL;
FILE *logs = stdout
_Bool exportResults = 0;
char *exportFileName = NULL;
_Bool bf = 0;
_Bool bfm = 0;
_Bool ppv = 0;
_Bool rw = 0;
_Bool twoOpt = 0;
_Bool ga = 0;
int ga_nSpecimens = 20;
int ga_nGenerations = 200;
double ga_mutationRate = 0.3;
*/

_Bool nz = 0;

// 1 if verbose mode is on, 0 if off. Default is 0
_Bool verbose = 0;

// points to log file. Default is stdout
extern FILE logFile;

// 1 if a TOUR file has been given
extern _Bool TOURflag;


int main(int argc, char **argv){
    args_t parsedArgs;
    memset( &(parsedArgs.TSPfileName), 0, MAXNAMELENGTH * sizeof(char) );
    memset( &(parsedArgs.TOURfileName), 0, MAXNAMELENGTH * sizeof(char) );
    memset( &(parsedArgs.logfileName), 0, MAXNAMELENGTH * sizeof(char) );
    parsedArgs.ga_nSpecimens = 20;
    parsedArgs.ga_nGenerations = 200;
    parsedArgs.ga_mutationRate = 0.3;


    int err;
    err = parseArguments(argc, argv, &parsedArgs);
    if( err < 0 ){
        fprintf(stderr, "ERROR : in main : parseArguments error\n");
        return -1;
    }
    else if( err == 0 ){
        printf("%s\n", HELP);
    }

    print_args(parsedArgs);


    FILE *fp = fopen("test.TSP", "r");
    if(fp == NULL){
        fprintf(stderr, "fopen failed\n");
        return -1;
    }

    instance_t instance;
    memset(instance.name, 0, MAXNAMELENGTH * sizeof(char));
    memset(instance.type, 0, MAXNAMELENGTH * sizeof(char));
    memset(instance.edge_type, 0, MAXNAMELENGTH * sizeof(char));
    instance.dimension = 0;
    instance.length = 0;

    err = parseFile(fp, &instance);
    if(err < 0){
        fprintf(stderr, "ERROR in main\n");
        return -1;
    }

    printf("\n\n-----INSTANCE-----\n");
    printf("Name: %s\n", instance.name);
    printf("Type: %s\n", instance.type);
    printf("Dimension: %d\n", instance.dimension);
    printf("Edge type: %s\n", instance.edge_type);
    printf("Length: %lf\n", instance.length);
    printf("tabCoord: \n");
    for(int i=0; i<instance.dimension; i++){
        printf("%d %d %d\n", i+1, instance.tabCoord[i][0], instance.tabCoord[i][1]);
    }
    printf("----------------------\n");

    instance.matDist = (double **) malloc(instance.dimension * sizeof(double *));
    if(instance.matDist == NULL){
        fprintf(stderr, "ERROR : in main : error while allocating matDist\n");
        exit(-1);
    }
    for(int i=0; i<instance.dimension; i++){
        instance.matDist[i] = (double *) malloc(instance.dimension * sizeof(double));
        if(instance.matDist[i] == NULL){
            fprintf(stderr, "ERROR : in main : error while allocating matDist");
            exit(-1);
        }
    }

    int *tourBuffer = (int *) malloc(instance.dimension * sizeof(int));
    if(tourBuffer == NULL){
        fprintf(stderr, "ERROR : in main : error while allocating tourBuffer\n");
        exit(-1);
    }

    fillMatDist(instance.matDist, instance.tabCoord, instance.dimension);

    bruteForce(&instance, tourBuffer);

    printf("Brute force:\n\n");
    for(int i=0; i<instance.dimension; i++){
        printf("%d ", tourBuffer[i]);
    }
    printf("\n\nBrute force avec matrice: \n\n");
    bruteForceMatrix(&instance, tourBuffer);
    for(int i=0; i<instance.dimension; i++){
        printf("%d ", tourBuffer[i]);
    }
    printf("\n");
    return 0;
}