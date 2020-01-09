#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "random_walk.h"

#include "TSP_parser.h"
#include "globals.h"
#include "brute_force.h"

/**
 * \file    random_walk.c
 * \brief   SOURCE -  Implements the random walk algorithm to solve a tsp problem
 * \author  BERNARD O.
 * \date    december 2019
 */

/// Implements the random walk algorithm to solve the tsp problem represented by the instance_t instance structure
/// Stores the best found tour in tourBuffer.
/// CALLS ABORT ON ALLOCATING ERRORS
double randomWalkSolver(instance_t *instance, int *tourBuffer){
    unsigned int dim = instance->dimension;
    int *tabTour = (int *) malloc(dim * sizeof(int)); // WHERE TO STORE THE TOUR
    if(tabTour == NULL){
        fprintf(stderr, "ERROR : in nearestNeighbourSolver : error while allocating tabTour\nAborting...\n");
        abort();
    }
    int *freeCities = (int *) malloc(dim * sizeof(int)); // TO KNOW WICH CITY HAS BEEN SELECTED
    if(freeCities == NULL){
        fprintf(stderr, "ERROR : in nearestNeighbourSolver : error while allocating freeCities\nAborting...\n");
        abort();
    }
    for(int i=0; i<dim; i++){ // freeCities[i] = 1 means node i is not yet selected, 0 otherwise
        freeCities[i] = 1;
    }

    time_t t;
    srand((unsigned) time(&t));


    for(int i=0; i<dim; i++){
        // CHOSE A CITY RANDOMLY
        int city = rand() % dim;
        while( freeCities[city] == 0 ){
            city = rand() % dim;
        }
        tabTour[i] = city;
        freeCities[city] = 0;
    }


    double length = tourLength(tabTour, instance->tabCoord, dim);

    memcpy(tourBuffer, tabTour, dim * sizeof(int));
    free(tabTour);
    free(freeCities);

    return length;
}