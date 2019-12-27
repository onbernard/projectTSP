#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "random_walk.h"

#include "TSP_parser_Q.h"
#include "globals.h"
#include "brute_force.h"


double randomWalkSolver(instance_t *instance, int *tourBuffer){
    unsigned int dim = instance->dimension;
    int *tabTour = (int *) malloc(dim * sizeof(int));
    if(tabTour == NULL){
        fprintf(stderr, "ERROR : in nearestNeighbourSolver : error while allocating tabTour\n");
        return -1;
    }
    int *freeNodes = (int *) malloc(dim * sizeof(int));
    if(freeNodes == NULL){
        fprintf(stderr, "ERROR : in nearestNeighbourSolver : error while allocating freeNodes\n");
        return -1;
    }
    for(int i=0; i<dim; i++){
        freeNodes[i] = 1;
    }

    time_t t;
    srand((unsigned) time(&t));


    for(int i=0; i<dim; i++){
        int pointA = rand() % dim;
        while( freeNodes[pointA] == 0 ){
            pointA = rand() % dim;

        }
        tabTour[i] = pointA;
        freeNodes[pointA] = 0;
    }


    double length = tourLength(tabTour, instance->tabCoord, dim);

    memcpy(tourBuffer, tabTour, dim * sizeof(int));
    free(tabTour);
    free(freeNodes);

    return length;
}