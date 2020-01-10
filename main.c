#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "globals.h"
#include "res.h"
#include "TSP_parser.h"
#include "TOUR_parser.h"
#include "arg_parser.h"
#include "brute_force.h"
#include "nearest_neighbour.h"
#include "random_walk.h"
#include "two_opt.h"
#include "genetic_algorithm.h"
#include "tools.h"
#include "csv.h"

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

    // =========================== PARSE ARGUMENTS ============================

    args_t args;
    init_args_t(&args);
    clock_t start, end;

    int err;
    err = parseArguments(argc, argv, &args);
    if(err <= 0){
        printf("\n%s\n", HELP);
    }
    if( err < 0 ){
        return -1;
    }
    // ========================================================================


    // =========================== FILES OPENING ==============================

    FILE *TSPfileP; // CLOSED JUST AFTER READ
    FILE *TOURfileP; // CLOSED JUST AFTER READ
    FILE *outfileP; // CLOSED AT THE END OF MAIN, AS IS LOGFILEP

    // ----------------- TSP FILE -----------------
    TSPfileP = fopen(args.TSPfileName, "r");
    if(TSPfileP == NULL){
        fprintf(stderr, "ERROR : in main : fopen failed for TSP file\n");
        return -1;
    }

    // ----------------- TOUR FILE ------------
    if(args.TOURfileName[0] != 0){
        TOURfileP = fopen(args.TOURfileName, "r");
        if(TOURfileP == NULL){
            fprintf(stderr, "ERROR : in main : fopen failed for tour file\n");
            return -1;
        }
    }

    // ------------------- LOGS ---------------
    if(args.logfileName[0] != 0){
        logfileP = fopen(args.logfileName, "w");
        if(logfileP == NULL){
            fprintf(stderr, "ERROR : in main : fopen failed for log file\n");
            return -1;
        }
    }
    else{
        logfileP = stdout;
    }

    // ------------- OUTPUT FILE ----------
    if(args.outputfileName[0] != 0){
        outfileP = fopen(args.outputfileName, "w");
        if(outfileP == NULL){
            fprintf(stderr, "ERROR : in main : fopen failed for output csv file\n");
            return -1;
        }
    }
    else{
        outfileP = stdout;
    }
    // ========================================================================


    // ========================== READ FILES ==================================
    if(verbose){
        fprintf(logfileP, "=========================== TSP PROGRAM ===========================\n\n");
        fprintf(logfileP, "Results of arguments parsing is (unspecified fields are empty) :\n");
        print_args(logfileP, args);
        fprintf(logfileP, "\nReading %s file...\n", args.TSPfileName);
    }

    instance_t TSPinstance;
    instance_t TOURinstance;
    // ------------ TSP -------------
    initTSPinstance(&TSPinstance);
    initTOURinstance(&TOURinstance);
    err = parseTSPfile(TSPfileP, &TSPinstance, args.nz);
    if(err < 0){
        exit(-1);
    }
    fclose(TSPfileP);
    int dim = TSPinstance.dimension;
    if(verbose){
        printTSPinstance(logfileP, TSPinstance);
    }
    // ------------ TOUR --------------
    if(args.TOURfileName[0] != 0){
        if(verbose){
            fprintf(logfileP, "Reading TOUR file...\n");
        }
        err = parseTOURfile(TOURfileP, &TOURinstance, args.nz); 
        if(err < 0){
            fprintf(logfileP, "ERROR : in main : parseTOURfile error\nContinuing without it...");
            if(TOURinstance.tabTour != NULL){
                free(TOURinstance.tabTour);
            }
            args.TOURfileName[0] = 0;
        }
        else{
            if(verbose){
                printTOURinstance(logfileP, TOURinstance);
            }
        }
        fclose(TOURfileP);
    }

    // ===========================================================

    // =========================== ALLOCATION ===========================
    TSPinstance.matDist = (double **) malloc(TSPinstance.dimension * sizeof(double *)); // dealoc
    if(TSPinstance.matDist == NULL){
        fprintf(stderr, "ERROR : in main : error while allocating matDist\n");
        exit(-1);
    }
    for(int i=0; i<TSPinstance.dimension; i++){
        TSPinstance.matDist[i] = (double *) malloc(TSPinstance.dimension * sizeof(double)); // dealoc
        if(TSPinstance.matDist[i] == NULL){
            fprintf(stderr, "ERROR : in main : error while allocating matDist");
            exit(-1);
        }
    }

    int *tourBuffer = (int *) malloc(TSPinstance.dimension * sizeof(int)); // dealoc
    if(tourBuffer == NULL){
        fprintf(stderr, "ERROR : in main : error while allocating tourBuffer\n");
        exit(-1);
    }
    // =====================================================================


    if(verbose){
        fprintf(logfileP, "Starting solving methods...\n\n");
    }
    // =========================== SOLVING ===================================
    double BFlength;
    double BFtime;
    if(args.solvingMethods & BF){

        start = clock();
        BFlength = bruteForce(&TSPinstance, tourBuffer);
        end = clock();

        BFtime = ((double)end-start)/CLOCKS_PER_SEC;
        fprintf(logfileP, "\nBrute force method found a tour with length %lf in %f secondes\n", BFlength, BFtime);
        if(verbose){
            printTour(tourBuffer, ' ', dim);
        }
        printRow(outfileP, TSPinstance.name, "BF", BFlength, BFtime, tourBuffer, dim);
    }

    double BFMlength;
    double BFMtime;
    if(args.solvingMethods & BFM){
        fillMatDist(TSPinstance.matDist, TSPinstance.tabCoord, TSPinstance.dimension);

        start = clock();
        BFMlength = bruteForceMatrix(&TSPinstance, tourBuffer);
        end = clock();

        BFMtime = ((double)end-start)/CLOCKS_PER_SEC;
        fprintf(logfileP, "\nBrute force with matrix method found a tour with length %lf in %f secondes\n", BFMlength, BFMtime);
        if(verbose){
            printTour(tourBuffer, ' ', dim);
        }
        printRow(outfileP, TSPinstance.name, "BFM", BFMlength, BFMtime, tourBuffer, dim);
    }

    double PPVlength;
    double PPVtime;
    if(args.solvingMethods & PPV){
        start = clock();
        PPVlength = nearestNeighbourSolver(&TSPinstance, tourBuffer);
        end = clock();
        PPVtime = ((double)end-start)/CLOCKS_PER_SEC;
        fprintf(logfileP, "\nNearest neighbour method found a tour with length %lf in %f secondes\n", PPVlength, PPVtime);
        if(verbose){
            printTour(tourBuffer, ' ', dim);
        }
        printRow(outfileP, TSPinstance.name, "PPV", PPVlength, PPVtime, tourBuffer, dim);
    }

    double RWlength;
    double RWtime;
    if(args.solvingMethods & RW){
        start = clock();
        RWlength = randomWalkSolver(&TSPinstance, tourBuffer);
        end = clock();
        RWtime = ((double)end-start)/CLOCKS_PER_SEC;
        fprintf(logfileP, "\nRandom walk method found a tour with length %lf in %f secondes\n", RWlength, RWtime);
        if(verbose){
            printTour(tourBuffer, ' ', dim);
        }
        printRow(outfileP, TSPinstance.name, "RW", RWlength, RWtime, tourBuffer, dim);
    }

    double TWOOPTlength;
    double TWOOPTtime;
    if(args.solvingMethods & TWOOPT){
        start = clock();
        TWOOPTlength = twoOptSolver(&TSPinstance, tourBuffer);
        end = clock();
        TWOOPTtime = ((double)end-start)/CLOCKS_PER_SEC;
        fprintf(logfileP, "\n2-opt method found a tour with length %lf in %f secondes\n", TWOOPTlength, TWOOPTtime);
        if(verbose){
            printTour(tourBuffer, ' ', dim);
        }
        printRow(outfileP, TSPinstance.name, "2OPT", TWOOPTlength, TWOOPTtime, tourBuffer, dim);
    }

    double GAlength;
    double GAtime;
    if(args.solvingMethods & GA){
        start = clock();
        GAlength = geneticAlgorithmSolver(&TSPinstance, tourBuffer, args.ga_nSpecimens, args.ga_nGenerations, args.ga_mutationRate);
        end = clock();
        GAtime = ((double)end-start)/CLOCKS_PER_SEC;
        fprintf(logfileP, "\nGenetic algorithm (nSpe=%d,nGe=%d,muR=%lf) method found a tour with length %lf in %f secondes\n", args.ga_nSpecimens, args.ga_nGenerations, args.ga_mutationRate, GAlength, GAtime);
        if(verbose){
            printTour(tourBuffer, ' ', dim);
        }
        printRow(outfileP, TSPinstance.name, "GA", GAlength, GAtime, tourBuffer, dim);
    }

    free(tourBuffer);
    for(int i=0; i<dim; i++){
        free(TSPinstance.matDist[i]);
    }
    free(TSPinstance.matDist);

    if(args.outputfileName[0] != 0){
        fclose(outfileP);
    }
    if(args.logfileName[0] != 0){
        fclose(logfileP);
    }

    return 0;
}