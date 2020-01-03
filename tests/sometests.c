#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "two_opt.h"

int main(int argc, char **argv){
    int **tabCoord = (int **) malloc(4*sizeof(int *));
    if(tabCoord == NULL){
        exit(-1);
    }
    for(int i=0; i<4; i++){
        tabCoord[i] = (int *) malloc(2*sizeof(int));
        if(tabCoord[i] == NULL){
            exit(-1);
        }
    }

    tabCoord[0][0] = 2233;
    tabCoord[0][1] = 1424;
    tabCoord[1][0] = 1112;
    tabCoord[1][1] = 2049;
    tabCoord[2][0] = 2233;
    tabCoord[2][1] = 10;
    tabCoord[3][0] = 7608;
    tabCoord[3][1] = 4458;

    int b = isCrossing(tabCoord, 0, 1, 2, 3);

    printf("%d\n", b);

    /*
    A[0][0] = 809.0;
    A[0][1] = 5375.0;
    A[0][2] = 6496.0;
    A[1][0] = -625.0;
    A[1][1] = 4448.0;
    A[1][2] = 2409.0;

    printSyst(A);

    L( A, 0, 1, -A[1][0]/A[0][0]);
    printSyst(A); printf("\n");
    M(A, 1, 1.0/A[1][1]);
    printSyst(A); printf("\n");

    double Tp = A[1][2];
    double T = ( A[0][2] - A[0][1]*Tp ) / A[0][0];

    printf("\n%lf %lf\n\n", T, Tp);
    */
}