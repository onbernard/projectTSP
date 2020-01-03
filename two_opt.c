#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "two_opt.h"

#include "TSP_parser_Q.h"
#include "brute_force.h"

double twoOpt(instance_t *instance, int *tabTour){
    int n = 1;
    int i = 0;
    while(n != 0 && i < 1000){
        n = doOneDecrossPass(instance, tabTour);
        printf("\nDone one pass. Number of crossing removed : %d\n\n", n);
        i++;
    }
    return tourLength(tabTour, instance->tabCoord, instance->dimension);
}

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

double absF(double x){
    if(x>0){
        return x;
    }
    else{
        return -x;
    }
}

_Bool isCrossing(int **tabCoord, int A, int B, int C, int D){

    if(A == D || A == D || B == C || B == D){
        return 0;
    }

    double mat[2][3];

    mat[0][0] = (double) (tabCoord[A][0] - tabCoord[B][0]);
    mat[0][1] = (double) (tabCoord[D][0] - tabCoord[C][0]);
    mat[0][2] = (double) (tabCoord[D][0] - tabCoord[B][0]);
    mat[1][0] = (double) (tabCoord[A][1] - tabCoord[B][1]);
    mat[1][1] = (double) (tabCoord[D][1] - tabCoord[C][1]);
    mat[1][2] = (double) (tabCoord[D][1] - tabCoord[B][1]);
    
    double det = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];

    if(  absF(det) <= 10E-100 ){
        return 0;
    }
    
    if( absF(mat[0][0]) <= 10E-100 ){
        P( mat, 0, 1);
    }
    L( mat, 0, 1, -mat[1][0]/mat[0][0]);
    M(mat, 1, 1.0/mat[1][1]);

    double Tp = mat[1][2];
    double T = ( mat[0][2] - mat[0][1]*Tp ) / mat[0][0];

    return Tp >= 0 && Tp <= 1 && T >= 0 && T <= 1;
}

void L(double mat[2][3], int l1, int l2, double m){
    for(int i=0; i<3; i++){
        mat[l2][i] += m* mat[l1][i];
    }
}

void M(double mat[2][3], int l, double m){
    for(int i=0; i<3; i++){
        mat[l][i] *= m;
    }
}

void P(double mat[2][3], int l1, int l2){
    double temp;
    for(int i=0; i<3; i++){
        temp = mat[l1][i];
        mat[l1][i] = mat[l2][i];
        mat[l2][i] = temp;
    }
}

void printSyst(double mat[2][3]){
    for(int i=0; i<2; i++){
        printf("| ");
        for(int j=0; j<3; j++){
            printf("%.2lf ", mat[i][j]);
        }
        printf("|\n");
    }
}
