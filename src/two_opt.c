#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "two_opt.h"

#include "globals.h"
#include "TSP_parser.h"
#include "random_walk.h"
#include "brute_force.h"

/**
 * \file    two_opt.c
 * \brief   SOURCE - Implements the 2-opt algorithm to solve a tsp problem
 * \author  BERNARD O.
 * \date    december 2019
 */


/// Implements the 2-opt algorithm on the tsp problem represented by the instance_t instance structure
/// Makes decross passes on the tour array and stops when either no crossing are left or a thousands passes
/// A decross pass is described in doOneDecrossPass documentation.
/// Stores the best found tour in the array pointed by tourBuffer and returns its length as a double.
/// CALLS ABORT ON ALLOCATION ERRORS
double twoOptSolver(instance_t *instance, int *tourBuffer){
    if(verbose){
        fprintf(logfileP, "========== IN 2OPT ==========\n");
    }

    int *tabTour = (int *) malloc(instance->dimension * sizeof(int)); // WHERE TO STORES THE TOUR
    if(tabTour == NULL){ // dealoc
        fprintf(stderr, "ERROR : in twoOpt : error while allocating tabTour\nAborting...\n");
        abort();
    }

    randomWalkSolver(instance, tabTour); // INITIALIZATION OF TOUR

    int n = 1;
    int i = 0;
    while(n != 0 && i < 1000){
        n = doOneDecrossPass(instance, tabTour);
        if(verbose){
            fprintf(logfileP, "Decross pass %d . Number of crossings removed : %d\n", i, n);
        }
        i++;
    }
    if(verbose){
        fprintf(logfileP, "========== OUT 2OPT ==========\n");
    }
    
    double length = tourLength(tabTour, instance->tabCoord, instance->dimension);


    memcpy(tourBuffer, tabTour, instance->dimension * sizeof(int));
    free(tabTour);

    return length;
}

/// Takes a tour array and for each segment i, check every segments j after i in the tour that is not connected to i
/// if it crosses i then the end value of i and the start value of j are swapped
/// One decrossing might generates another crossing somewhere else but the function does not check for that in order
/// to not get stuck in an infinite loop
int doOneDecrossPass(instance_t *instance, int *tabTour){
    int dim = instance->dimension;
    int n = 0;
    for(int i=1; i<dim; i++){
        for(int j=i+2; j<dim; j++){
            if( isCrossing(instance->tabCoord, tabTour[i-1], tabTour[i], tabTour[j-1], tabTour[j]) ){
                n++;
                int temp = tabTour[i];
                tabTour[i] = tabTour[j-1];
                tabTour[j-1] = temp;
            }
        }
    }
    return n;
}

/// For debugging purposes
/// Goes through the tour and shouts when a segment intersect another
int numberOfCrossing(instance_t *instance, int *tabTour){
    int dim = instance->dimension;
    int **tabCoord = instance->tabCoord;
    int n = 0;
    for(int i=1; i<dim; i++){
        for(int j=i+2; j<dim; j++){
            if( isCrossing(instance->tabCoord, tabTour[i-1], tabTour[i], tabTour[j-1], tabTour[j]) ){
                n++;
                int A = tabTour[i-1];
                int B = tabTour[i];
                int C = tabTour[j-1];
                int D = tabTour[j];
                printf("City %d to city %d : (%d , %d) to (%d , %d) \ncrosses\n", A, B, tabCoord[1][0], tabCoord[A][1], tabCoord[B][0], tabCoord[B][1]);
                printf("City %d to city %d : (%d , %d) to (%d , %d)\n\n", C, D, tabCoord[C][0], tabCoord[C][1], tabCoord[D][0], tabCoord[D][1]);
            }
        }
    }
    return n;
}

/// Returns the absolute value of a double. Because math.h does not have that apparently
double absF(double x){
    if(x>0){
        return x;
    }
    else{
        return -x;
    }
}

/// Returns 1 if segment going from cityA to cityB crosses the segment going grom cityC to cityD
/// Does it by representing AB and CD as a line with parameters t and t' such that
///
///     AB(t) = A(t) + B(1-t)
///     CD(t') = C(t') + D(1-t')
///
/// Crossing = (t,t') := AB(t) = CD(t')
///
/// If 0 <= t <= 1   and 0 <=t' <= 1        then the segments crosses
///
/// To compute (t,t') it solves the system of equations :
///
///     | Ax-Bx     Dx-Cx | | t |  ---  | Dx-Bx |
///     | Ay-By     Dy-Cy | | t'|  ---  | Dy-By |
///
/// DUE TO NUMERICAL INSTABILITY, THIS FUNCTION IS HAZARDOUS AT BEST, USE SMALL OR SIMILAR NUMBERS AT YOU OWN RISKS
_Bool isCrossing(int **tabCoord, int A, int B, int C, int D){
    // the cities must be differents
    if(A == D || A == D || B == C || B == D){
        return 0;
    }

    // system matrix
    double mat[2][3];
    // filling it up
    mat[0][0] = (double) (tabCoord[A][0] - tabCoord[B][0]);
    mat[0][1] = (double) (tabCoord[D][0] - tabCoord[C][0]);
    mat[0][2] = (double) (tabCoord[D][0] - tabCoord[B][0]);
    mat[1][0] = (double) (tabCoord[A][1] - tabCoord[B][1]);
    mat[1][1] = (double) (tabCoord[D][1] - tabCoord[C][1]);
    mat[1][2] = (double) (tabCoord[D][1] - tabCoord[B][1]);
    
    // determine the determined determinant
    double det = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
    // if it is not well determined, do not go further
    if(  absF(det) <= 10E-100 ){
        return 0;
    }
    
    if( absF(mat[0][0]) <= 10E-100 ){ // so that we do not divide by zero
        P( mat, 0, 1); // permute
    }
    L( mat, 0, 1, -mat[1][0]/mat[0][0]); // gaussian elimination on the second line
    M(mat, 1, 1.0/mat[1][1]); // so that we have 1 on the rightmost column

    double Tp = mat[1][2]; // we can harvest the first value
    double T = ( mat[0][2] - mat[0][1]*Tp ) / mat[0][0]; // and use it to get the second

    return Tp >= 0 && Tp <= 1 && T >= 0 && T <= 1; // cross or not
}


/// Implements a linear combination of the lines of a matrix
/// L2 <--- L2 + L1 * m
void L(double mat[2][3], int l1, int l2, double m){ 
    for(int i=0; i<3; i++){
        mat[l2][i] += m* mat[l1][i];
    }
}

/// Multiply a line l by m
void M(double mat[2][3], int l, double m){ //
    for(int i=0; i<3; i++){
        mat[l][i] *= m;
    }
}

/// Permutes lines l1 and l2 of mat
void P(double mat[2][3], int l1, int l2){
    double temp;
    for(int i=0; i<3; i++){
        temp = mat[l1][i];
        mat[l1][i] = mat[l2][i];
        mat[l2][i] = temp;
    }
}

/// For debugging purposes, prints a 2*3 double matrix
void printSyst(double mat[2][3]){
    for(int i=0; i<2; i++){
        printf("| ");
        for(int j=0; j<3; j++){
            printf("%.2lf ", mat[i][j]);
        }
        printf("|\n");
    }
}
