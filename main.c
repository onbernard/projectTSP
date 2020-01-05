#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "res.h"
#include "TSP_parser.h"
#include "TOUR_parser.h"
#include "arg_parser.h"
/*
#include "brute_force.h"
#include "nearest_neighbour.h"
#include "random_walk.h"
#include "two_opt.h"
#include "genetic_algorithm.h"
*/

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

// 1 if verbose mode is on, 0 if off. Default is 0
_Bool verbose = 0;

// points to log file. Default is stdout
FILE *logfileP;


int main(int argc, char **argv){
    // ================== PARSE ARGUMENTS ===================
    args_t args;
    init_args_t(&args);

    int err;
    err = parseArguments(argc, argv, &args);
    if(err <= 0){
        printf("\n%s\n", HELP);
    }
    if( err < 0 ){
        return -1;
    }

    FILE *TSPfileP;
    FILE *TOURfileP;
    FILE *outfileP;

    // ================== OPEN TSP FILE ====================
    TSPfileP = fopen(args.TSPfileName, "r");
    if(TSPfileP == NULL){
        fprintf(stderr, "ERROR : in main : fopen failed for TSP file\n");
        return -1;
    }

    // ==================== OPEN TOUR FILE =====================
    if(args.TOURfileName[0] != 0){
        TOURfileP = fopen(args.TOURfileName, "r");
        if(TOURfileP == NULL){
            fprintf(stderr, "ERROR : in main : fopen failed for tour file\n");
            return -1;
        }
    }

    // ================= LOGS ======================
    if(args.logfileName[0] != 0){
        logfileP = fopen(args.logfileName, "w");
        if(logfileP == NULL){
            fprintf(stderr, "ERROR : in main : fopen failed for log file");
            return -1;
        }
    }
    else{
        logfileP = stdout;
    }

    // ================== OPEN OUT CSV FILE ==================
    if(args.outputfileName[0] != 0){
        outfileP = fopen(args.outputfileName, "w");
        if(outfileP == NULL){
            fprintf(stderr, "ERROR : in main : fopen failed for output csv file\n");
            return -1;
        }
    }

    // ================== READ TSP FILE ==========================
    instance_t TSPinstance;
    instance_t TOURinstance;
    initTSPinstance(&TSPinstance);
    initTOURinstance(&TOURinstance);
    err = parseTSPfile(TSPfileP, &TSPinstance, args.nz);
    if(err < 0){
        return -1;
    }
    // ==================== READ TOUR FILE ==========================
    if(args.TOURfileName[0] != 0){
        err = parseTOURfile(TOURfileP, &TOURinstance, args.nz);
        if(err < 0){
            return -1;
        }/*
        else if( TOURinstance.dimension != TSPinstance.dimension){
            fprintf(stderr, "ERROR : in main : dimensions of TSP and TOUR instances do not correspond (%d vs %d)\n", TSPinstance.dimension, TOURinstance.dimension);
            return -1;
        }*/
        //TOURinstance.length = tourLength(TOURinstance.tabTour, TSPinstance.tabCoord, TSPinstance.dimension);
    }


    printf("\n\n========== TSP INSTANCE %s ==========\n", TSPinstance.name);
    printf("Name :              %s\n", TSPinstance.name);
    printf("Type :              %s\n", TSPinstance.type);
    printf("Dimension :         %d\n", TSPinstance.dimension);
    printf("Edge type :         %s\n", TSPinstance.edge_type);
    printf("Length :            %lf\n", TSPinstance.length);
    printf("tabCoord : \n");
    for(int i=0; i<TSPinstance.dimension; i++){
        printf("%d      %d      %d\n", i, TSPinstance.tabCoord[i][0], TSPinstance.tabCoord[i][1]);
    }
    printf("===============================================\n\n");

    if(args.TOURfileName[0] != 0){
        printf("\n\n========== TOUR INSTANCE %s ==========\n", TOURinstance.name);
        printf("Name :              %s\n", TOURinstance.name);
        printf("Type :              %s\n", TOURinstance.type);
        printf("Dimension :         %d\n", TOURinstance.dimension);
        printf("Edge type :         %s\n", TOURinstance.edge_type);
        printf("Length :            %lf\n", TOURinstance.length);
        printf("tabTour : \n");
        for(int i=0; i<TOURinstance.dimension; i++){
            printf("%d      %d\n", i, TOURinstance.tabTour[i]);
        }
        printf("===============================================\n\n");
    }

    /*
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

    double length = bruteForce(&instance, tourBuffer);

    printf("Brute force:\n\n");
    for(int i=0; i<instance.dimension; i++){
        printf("%d ", tourBuffer[i]);
    }
    printf("\nLength = %lf\n", length);

    printf("\n\nBrute force avec matrice: \n\n");
    length = bruteForceMatrix(&instance, tourBuffer);
    for(int i=0; i<instance.dimension; i++){
        printf("%d ", tourBuffer[i]);
    }
    printf("\nLength = %lf\n", length);

    length = nearestNeighbourSolver(&instance, tourBuffer);
    printf("\n\nNearest neighbour solver: \n\n");

    for(int i=0; i<instance.dimension; i++){
        printf("%d ", tourBuffer[i]);
    }
    printf("\nLength = %lf\n\n", length);

    length = randomWalkSolver(&instance, tourBuffer);
    printf("\n\nRandom walk solver: \n\n");
    for(int i=0; i<instance.dimension; i++){
        printf("%d ", tourBuffer[i]);
    }
    printf("\nLength = %lf\n\n", length);

    printf("\nTWO OPT\n");
    length = twoOpt(&instance, tourBuffer);
    for(int i=0; i<instance.dimension; i++){
        printf("%d ", tourBuffer[i]);
    }
    printf("\nLength = %lf\n\n", length);

    printf("\nGENETIC ALGORITHM\n");
    length = geneticAlgorithmSolver(&instance, tourBuffer, 30, 200, 0.3);
    for(int i=0; i<instance.dimension; i++){
        printf("%d ", tourBuffer[i]);
    }
    printf("\nLength = %lf\n\n", length);
    */

    return 0;
}