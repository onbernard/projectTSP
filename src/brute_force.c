#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "brute_force.h"

#include "globals.h"
#include "TSP_parser.h"

/**
 * \file    brute_force.c
 * \brief   SOURCE - Implements the brute force methode either with a distance matrix or not. 
 * \author  BERNARD O.
 * \date    december 2019
 */

/// Implements the brute force algorithm on a tsp problem using a pre-generated distance matrix.
/// IT DOES NOT GENERATE IT ITSELF!. It uses an array where it cycles through 
/// every possible permutation and then stores the best one in the buffer passed as second argument.
/// CALLS ABORT ON ALLOCATION ERRORS
double bruteForceMatrix(instance_t *instance, int *tourBuffer){
    if(verbose){
        fprintf(logfileP, "========== IN BF ==========\n");
    }

    unsigned dim = instance->dimension;



    double length = 0;
    double minLength = 0;
    

    // WHERE TO STORE THE CURRENT BEST
    int *minTour = (int *) malloc( dim * sizeof(int) );
    if(minTour == NULL){
        fprintf(stderr, "ERROR : in bruteForceMatrix : error while allocating minTour.\nAborting...\n");
        abort();
    }
    // WHERE TO STORE THE TOURS
    int *tabTour = (int *) malloc( dim * sizeof(int) );
    if(tabTour == NULL){
        fprintf(stderr, "ERROR : in bruteForceMatrix : error while allocating tabTour.\nAborting...\n");
        abort();
    }
    // PRIMING THE CYCLE
    for(int i=0; i<dim; i++){
        tabTour[i] = i;
    }
    minLength = tourLengthMatrix(tabTour, instance->matDist, instance->dimension);

    int end = 0;
    unsigned long long int n=1;
    while( !end ){ // CYCLES THROUGH EVERY COMBINATION
        length = tourLengthMatrix(tabTour, instance->matDist, instance->dimension);
        if(length < minLength ){
            minLength = length;
            memcpy(minTour, tabTour, dim*sizeof(tabTour[0]));
        }
        end = next(tabTour, dim);
        n++;
    }
 
    // COPY INTO BUFFER
    memcpy(tourBuffer, minTour, dim*sizeof(minTour[0]));
    free(minTour);
    free(tabTour);
    if(verbose){
        fprintf(logfileP, "With %d cities, %llu permutations were checked (this number might overflow)\n", dim, n);
        fprintf(logfileP, "========== OUT BF ==========\n");
    }
    return minLength;
}

/// Implements the brute force algorithm on a tsp problem calculating the distances on the fly.
/// It uses an array where it cycles through every possible permutation and then stores the 
/// best one in the buffer passed as second argument.
/// CALLS ABORT ON ALLOCATING ERRORS
double bruteForce(instance_t *instance, int *tourBuffer){
    // basically a copy of the one above but i don't like functions with too many arguments so now there's to of em
    unsigned dim = instance->dimension;

    double length = 0;
    double minLength = 0;

    // WHERE TO STORE THE CURRENT BEST
    int *minTour = (int *) malloc( dim * sizeof(int) );
    if(minTour == NULL){
        fprintf(stderr, "ERROR : in bruteForceMatrix : error while allocating minTour.\nAborting...\n");
        abort();
    }
    // WHERE TO STORE THE TOURS
    int *tabTour = (int *) malloc( dim * sizeof(int) );
    if(tabTour == NULL){
        fprintf(stderr, "ERROR : in bruteForceMatrix : error while allocating tabTour.\nAborting...\n");
        abort();
    }
    // PRIMING THE CYCLE
    for(int i=0; i<dim; i++){
        tabTour[i] = i;
    }
    minLength = tourLength(tabTour, instance->tabCoord, instance->dimension);

    int end = 0;
    while( !end ){ // CYCLES THROUGH EVERY COMBINATION
        length = tourLength(tabTour, instance->tabCoord, instance->dimension);
        if(length < minLength ){
            minLength = length;
            memcpy(minTour, tabTour, dim*sizeof(tabTour[0]));
        }
        end = next(tabTour, dim);
    }

    // COPY INTO BUFFER
    memcpy(tourBuffer, minTour, dim*sizeof(minTour[0]));
    free(minTour); // MURICA
    free(tabTour); // LAND OF THE DEALLOC
    return minLength;
}

/// Fills the PREALLOCATED matdist with the distances between every city
/// so that matDist[i][j] = matDist[j][i] is the distance between city i and city j
void fillMatDist(double **matDist, int **tabCoord, unsigned int size){
    for(int i=0; i<size; i++){
        for(int j=i; j<size; j++){
            if(i==j){
                matDist[i][i] = 0;
            }
            else {
                matDist[i][j] = sqrt( pow( tabCoord[i][0] - tabCoord[j][0], 2 ) + pow( tabCoord[i][1] - tabCoord[j][1], 2 ) );
                matDist[j][i] = matDist[i][j];
            }
        }
    }
}

/// Computes the length of the tour passed in tabTour by calculating every ditances useing the coordinates array.
/// Last index in the tour is considered to be connected to the first one.
double tourLength(int *tabTour, int **tabCoord, unsigned int size){
    
    double length = 0;

    double prevX = tabCoord[tabTour[0]][0];
    double prevY = tabCoord[tabTour[0]][1];
    double curX;
    double curY;

    for(int i=1; i<size; i++){
        curX = tabCoord[tabTour[i]][0];
        curY = tabCoord[tabTour[i]][1];
        length += sqrt( pow( prevX-curX , 2) + pow( prevY-curY , 2) );
        prevX = curX;
        prevY = curY;
    }
    length += sqrt( pow( curX-tabCoord[tabTour[0]][0] , 2) + pow( curY-tabCoord[tabTour[0]][1] , 2) );
    return length;
}

/// Computes the length of the tour passed in tabTour using the matrix of distances passed in matDist.
/// Last index in the tour is considered to be connected to the first one.
double tourLengthMatrix(int *tabTour, double **matDist, unsigned int size){
    double length = 0;

    for(int i=1; i<size; i++){
        length += matDist[tabTour[i-1]][tabTour[i]];
    }

    length += matDist[tabTour[0]][tabTour[size-1]];

    return length;

}

/// Takes an array filled with integers from 0 to size-1 and generate the next permutation in the lexicographic
/// order. If the permutation passed is the last one, it does not change it and returns 1. Otherwise, it overwrites
/// tabTour and returns 0;
int next(int *tabTour, unsigned int size){

    int i = size - 2;

    while( i >= 0  ){
        if(tabTour[i] < tabTour[i+1]){
            break;
        }
        i--;
    }
    if( i == -1 ){
        return 1;
    }
    int j = i+1;
    int sec = i +1;
    while( j<size ){
        if( tabTour[j] > tabTour[i] && tabTour[j] < tabTour[sec] ){
            sec = j;
        }
        j++;
    }
    swap(tabTour, i, sec);
    qsort( tabTour + i + 1, size - i - 1, sizeof(tabTour[0]), compare ); 
    
    return 0;
}

/// Used in next to sort the array
int compare(const void *a, const void *b){
    return *((int *) a) - *((int *) b);
}

/// Used in next to ... wait for it ... swap two elements! I keep comin' up with 'em don't i?
/// i<size && j<size !!!!!!s
void swap(int *tab, int i, int j){
    int temp = tab[i];
    tab[i] = tab[j];
    tab[j] = temp;
    return;
}