#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "brute_force.h"

#include "globals.h"
#include "TSP_parser_Q.h"

extern _Bool nz;

void bruteForceMatrix(instance_t *instance, int *tourBuffer){
    unsigned dim = instance->dimension;

    double length = 0;
    double minLength = 0;

    int *minTour = (int *) malloc( dim * sizeof(int) );
    if(minTour == NULL){
        fprintf(stderr, "ERROR : in bruteForceMatrix : error while allocating minTour\n");
        exit(-1);
    }
    int *tabTour = (int *) malloc( dim * sizeof(int) );
    if(tabTour == NULL){
        fprintf(stderr, "ERROR : in bruteForceMatrix : error while allocating tabTour\n");
        exit(-1);
    }
    for(int i=0; i<dim; i++){
        tabTour[i] = i;
    }
    minLength = tourLengthMatrix(tabTour, instance->tabCoord, instance->matDist, instance->dimension);

    int end = 0;
    while( !end ){
        length = tourLengthMatrix(tabTour, instance->tabCoord, instance->matDist, instance->dimension);
        if(length < minLength ){
            minLength = length;
            memcpy(minTour, tabTour, dim*sizeof(tabTour[0]));
        }
        end = next(tabTour, dim);
    }
 
    memcpy(tourBuffer, minTour, dim*sizeof(minTour[0]));
}

void bruteForce(instance_t *instance, int *tourBuffer){
    unsigned dim = instance->dimension;

    double length = 0;
    double minLength = 0;

    int *minTour = (int *) malloc( dim * sizeof(int) );
    if(minTour == NULL){
        fprintf(stderr, "ERROR : in bruteForceMatrix : error while allocating minTour\n");
        exit(-1);
    }
    int *tabTour = (int *) malloc( dim * sizeof(int) );
    if(tabTour == NULL){
        fprintf(stderr, "ERROR : in bruteForceMatrix : error while allocating tabTour\n");
        exit(-1);
    }
    for(int i=0; i<dim; i++){
        tabTour[i] = i;
    }
    minLength = tourLength(tabTour, instance->tabCoord, instance->dimension);

    int end = 0;
    while( !end ){
        length = tourLength(tabTour, instance->tabCoord, instance->dimension);
        if(length < minLength ){
            minLength = length;
            memcpy(minTour, tabTour, dim*sizeof(tabTour[0]));
        }
        end = next(tabTour, dim);
    }

    memcpy(tourBuffer, minTour, dim*sizeof(minTour[0]));
}

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

double tourLength(int *tabTour, int **tabCoord, unsigned int size){
    double length = 0;

    double prevX = tabCoord[tabTour[0]][0];
    double prevY = tabCoord[tabTour[0]][1];

    double curX;
    double curY;

    if(!nz){
        length += sqrt( pow( prevX , 2) + pow( prevY , 2) );
    }

    for(int i=1; i<size; i++){
        curX = tabCoord[tabTour[i]][0];
        curY = tabCoord[tabTour[i]][1];
        length += sqrt( pow( prevX-curX , 2) + pow( prevY-curY , 2) );
        prevX = curX;
        prevY = curY;
    }

    if(nz){
        length += sqrt( pow(prevX - tabCoord[tabTour[0]][0], 2) + pow(prevY - tabCoord[tabTour[0]][1], 2) );
    }
    else{
        length += sqrt( pow(prevX, 2) + pow(prevY, 2) );
    }

    return length;
}

double tourLengthMatrix(int *tabTour, int **tabCoord, double **matDist, unsigned int size){
    double length = 0;

    if(!nz){
        length += sqrt( pow( tabCoord[tabTour[0]][0] , 2) + pow( tabCoord[tabTour[0]][1] , 2) );
    }

    for(int i=1; i<size; i++){
        length += matDist[tabTour[i-1]][tabTour[i]];
    }

    if(nz){
        length += matDist[tabTour[size-1]][tabTour[0]];
    }
    else{
        length += sqrt( pow(tabCoord[tabTour[size-1]][0], 2) + pow(tabCoord[tabTour[size-1]][1], 2) );
    }

    return length;

}

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

int compare(const void *a, const void *b){
    return *((int *) a) - *((int *) b);
}

void swap(int *tab, int i, int j){
    int temp = tab[i];
    tab[i] = tab[j];
    tab[j] = temp;
    return;
}