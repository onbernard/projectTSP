#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "nearest_neighbour.h"


#include "TSP_parser_Q.h"
#include "globals.h"
#include "brute_force.h"

double nearestNeighbourSolver(instance_t *instance, int *tourBuffer){
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

    if(nz){
        tabTour[0] = 0;
        freeNodes[0] = 0;
    }
    else{
        tabTour[0] = nearestToZero(instance->tabCoord, dim);
        freeNodes[tabTour[0]] = 0;
    }
    int pointA = tabTour[0];
    for(int i=1; i<dim; i++){
        int pointB = findNearestNeighbour(pointA, instance->tabCoord, dim, freeNodes);
        pointB = findNearestNeighbour(pointA, instance->tabCoord, dim, freeNodes);
        tabTour[i] = pointB;
        freeNodes[pointB] = 0;
        pointA = pointB;
    }

    double length = tourLength(tabTour, instance->tabCoord, dim);

    memcpy(tourBuffer, tabTour, dim * sizeof(int));
    free(tabTour);
    free(freeNodes);

    return length;
}

int findNearestNeighbour(int node, int **tabCoord, unsigned int size, int *freeNodes){
    double minDist;
    int iMinDist;


    if(node == 0){
        minDist = distance(node, 1, tabCoord);
        iMinDist = 1;
    }
    else{
        minDist = distance(node, 0, tabCoord);
        iMinDist = 0;
    }
    for(int i=0; i<size; i++){
        if( i != node && freeNodes[i] != 0 ){
            int dist = distance(node, i, tabCoord);
            if(dist < minDist){
                iMinDist = i;
                minDist = dist;
            }
        }
    }
    freeNodes[iMinDist] = 0;
    return iMinDist;
}

int nearestToZero(int **tabCoord, unsigned int size){
    int minDist = distanceFromZero(0, tabCoord);
    int iMinDist = 0;
    for(int i=1; i<size; i++){
        double dist = distanceFromZero(i, tabCoord);
        if(dist < minDist){
            minDist = dist;
            iMinDist = i;
        }
    }
    return iMinDist;
}

double distanceFromZero(int node, int **tabCoord){
    return sqrt( pow(tabCoord[node][0], 2) + pow(tabCoord[node][1], 2) );
}

double distance(int pointA, int pointB, int **tabCoord){
    return sqrt( pow(tabCoord[pointA][0] - tabCoord[pointB][0], 2) + pow(tabCoord[pointA][1] - tabCoord[pointB][1], 2) );
}

