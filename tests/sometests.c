#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "two_opt.h"

/* found in ga
/// Returns 1 if city node is to the left or right of index index in array tour
int isNeighbour(int index, int node, int *tabTour, unsigned int size){
    if( tabTour[(index+1)%size] == node ){
        return (index+1) % size;
    }
    if(index == 0){
        index = size;
    }
    if( tabTour[index-1] == node ){
        return index-1;
    }
    return -1;
}



void test1(){
    int A[10] = {5, 9, 3, 1, 2, 8, 0, 6, 7, 4};
    int B[10] = {1, 2, 5, 3, 9, 4, 8, 6, 0, 7};
    int C[10];
    int tabCoord[10][2] = { {22, 43}, {45, 65}, {59, 53}, {76, 93}, {44, 64}, {765, 43}, {43, 63}, {60, 54}, {61, 55}, {62, 56} };
    DPX(tabCoord, A, B, C, 10);

    printf("\n");
    for(int i=0; i<10; i++){
        printf("%d %d\n", tabCoord[i][0], tabCoord[i][1]);
    }
    printf("\n\n");

    printf("\n");
    for(int i=0; i<10; i++){
        printf("%d ", A[i]);
    }
    printf("\n");

    printf("\n");
    for(int i=0; i<10; i++){
        printf("%d ", B[i]);
    }
    printf("\n");

    printf("\n");
    for(int i=0; i<10; i++){
        printf("%d ", C[i]);
    }
    printf("\n");
}*/

void test2(){
    int A[10] = {5, 9, 3, 1, 2, 8, 0, 6, 7, 4};
    int B[10] = {3, 2, 1, 8, 0, 6, 7, 4, 5, 9};
    int C[10];
    int tabCoord[10][2] = { {22, 43}, {45, 65}, {59, 53}, {76, 93}, {44, 64}, {765, 43}, {43, 63}, {60, 54}, {61, 55}, {62, 56} };
    DPX(tabCoord, A, B, C, 10);

    printf("\n");
    for(int i=0; i<10; i++){
        printf("%d %d\n", tabCoord[i][0], tabCoord[i][1]);
    }
    printf("\n\n");

    printf("\n");
    for(int i=0; i<10; i++){
        printf("%d ", A[i]);
    }
    printf("\n");

    printf("\n");
    for(int i=0; i<10; i++){
        printf("%d ", B[i]);
    }
    printf("\n");

    printf("\n");
    for(int i=0; i<10; i++){
        printf("%d ", C[i]);
    }
    printf("\n");
}

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