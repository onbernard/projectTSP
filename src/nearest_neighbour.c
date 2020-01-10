#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "nearest_neighbour.h"

#include "TSP_parser.h"
#include "globals.h"
#include "brute_force.h"

/**
 * \file    nearest_neighbour.c
 * \brief   SOURCE -  Implements the nearest neighbour algorithm to solve a tsp problem
 * \author  BERNARD O.
 * \date    december 2019
 */

/// Implements the nearest neighbour algorithm to solve the tsp problem represented by the instance_t instance structure
/// Stores the best found tour in the array pointed by tourBuffer and returns its length as a double.
/// CALLS ABORT ON ALLOCATING ERRORS
double nearestNeighbourSolver(instance_t *instance, int *tourBuffer){
    unsigned int dim = instance->dimension;

    int *tabTour = (int *) malloc(dim * sizeof(int)); // WHERE TO STORES THE TOURS // dealoc
    if(tabTour == NULL){
        fprintf(stderr, "ERROR : in nearestNeighbourSolver : error while allocating tabTour\nAborting...\n");
        abort();
    }
    int *freeCities = (int *) malloc(dim * sizeof(int)); // TO KNOW WICH CITY HAS ALREADY BEEN SELECTED
    if(freeCities == NULL){ // dealoc
        fprintf(stderr, "ERROR : in nearestNeighbourSolver : error while allocating freeCities\nAborting...\n");
        abort();
    }
    for(int i=0; i<dim; i++){
        freeCities[i] = 1; // freeCities[i] = 1 means city i is not yet selected, 0 otherwise
    }

    // first one is always node 0
    tabTour[0] = 0;
    freeCities[0] = 0;

    int cityA = tabTour[0];
    for(int i=1; i<dim; i++){
        int cityB = findNearestNeighbour(cityA, instance->tabCoord, freeCities, dim); // FETCH CLOSEST NEIGHBOUR
        tabTour[i] = cityB; // TAKE A SEAT YOUNG SKYWALKER
        freeCities[cityB] = 0; // NOT FREE ANYMORE
        cityA = cityB; // swipity swapity your city is now spagetti (⋆._.)⊃▁⛥⌒*ﾟ.❉・゜・。.
    }

    double length = tourLength(tabTour, instance->tabCoord, dim);

    memcpy(tourBuffer, tabTour, dim * sizeof(int));
    free(tabTour);
    free(freeCities);

    return length;
}

/// Finds the city that is closest to cityA that is not yet selected
/// FREECITIES CONTAINS AT LEAST ONE FREE CITY, unspecified behavious otherwise...
int findNearestNeighbour(int cityA, int **tabCoord, int *freeCities,  unsigned int size){
    double minDist;
    int iMinDist = -1;


    for(int i=0; i<size; i++){
        if( i != cityA && freeCities[i] != 0 ){
            int dist = distance(cityA, i, tabCoord);
            if(iMinDist == -1 || dist < minDist){
                iMinDist = i;
                minDist = dist;
            }
        }
    }
    return iMinDist;
}

/// Computes the distance between cityA and cityB using the coordinates two dimensionnal array tabCoord
double distance(int cityA, int cityB, int **tabCoord){
    return sqrt( pow(tabCoord[cityA][0] - tabCoord[cityB][0], 2) + pow(tabCoord[cityA][1] - tabCoord[cityB][1], 2) );
}

