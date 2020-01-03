#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "genetic_algorithm.h"

#include "brute_force.h"
#include "globals.h"
#include "TSP_parser_Q.h"
#include "random_walk.h"
#include "nearest_neighbour.h"




double geneticAlgorithmSolver(instance_t *instance, int *tourBuffer, int nSpecimens, int nGenerations, double mutationRate){
    int dim = instance->dimension;
    time_t t;
    srand((unsigned) time(&t));
    double worseLength = -1;
    int worseIndex = -1;

    // INITIALIZATION OF POPULATION
    double *lengthsArr = (double *) malloc(nSpecimens * sizeof(double));
    if(lengthsArr == NULL){
        fprintf(stderr, "ERROR : in geneticAlgorithmSolver : error while allocating lengthsArr\n");
        return -1;
    }
    int **population = (int **) malloc(nSpecimens * sizeof(int *));
    if(population == NULL){
        fprintf(stderr, "ERROR : in geneticAlgorithmSolver : error while allocating population\n");
        return -1;
    }
    for(int i=0; i<nSpecimens; i++){
        population[i] = (int *) malloc(dim * sizeof(int));
        if(population[i] == NULL){
            fprintf(stderr, "ERROR : in geneticAlgorithmSolver : error while allocating population[%d]\n", i);
            return -1;
        }
        randomWalkSolver(instance, population[i]);
        lengthsArr[i] = tourLength(population[i], instance->tabCoord, dim);
        if(worseIndex == -1 || lengthsArr[i] > worseLength){
            worseIndex = i;
            worseLength = lengthsArr[i];
        }
    }

    int *childBuffer = (int *) malloc(dim * sizeof(int));
    if(childBuffer == NULL){
        fprintf(stderr, "ERROR : in geneticAlgorithmSolver : error while allocating childBuffer\n");
        return -1;
    }    

    // ITERATE OVER GENERATIONS
    for(int i=0; i<nGenerations; i++){
        int numberOfCrossBreeding = rand() % (nSpecimens/2); // NOT EVERYONE GETS LAID
        for(int k=0; k<numberOfCrossBreeding; k++){ // BREEDING SEASON!!!!
            int parent1 = rand() % nSpecimens; // YOU GET A BABY
            int parent2 = rand() % nSpecimens; // YOU GET A BABY
            while(parent2 == parent1){
                parent2 = rand() % dim;
            }
            if( DPX(instance->tabCoord, population[parent1], population[parent2], childBuffer, dim) < 0 ){
                fprintf(stderr, "ERROR : in geneticAlgorithmSolver : error in DPX\n");
                return -1;
            }

            if(rollDice(mutationRate)){ // OH NO ITS RETARDED
                mute(childBuffer, dim);
            }
            
            // MOVE ASIDE GRANDPA
            double childLength = tourLength(childBuffer, instance->tabCoord, dim);
            memcpy(population[worseIndex], childBuffer, dim *sizeof(int));
            lengthsArr[worseIndex] = childLength;
            worseIndex = iMaxInArray(lengthsArr, nSpecimens);
            worseLength = lengthsArr[worseIndex];
        }
    }

    int iBestOfPopulation = iMinInArray(lengthsArr, nSpecimens);
    double bestLength = lengthsArr[iBestOfPopulation];
    memcpy(tourBuffer, population[iBestOfPopulation], dim * sizeof(int));

    free(lengthsArr);
    for(int i=0; i<nSpecimens; i++){
        free(population[i]);
    }
    free(childBuffer);
    free(population); // DEBOUT LES DAMNES DE LA TEEEEEEEREUH
    return bestLength;
}

int iMinInArray(double *arr, unsigned int size){
    double min = arr[0];
    int iMin = 0;
    for(int i=0; i<size; i++){
        if(arr[i] < min){
            min = arr[i];
            iMin = i;
        }
    }
    return iMin;
}

int iMaxInArray(double *arr, unsigned int size){
    double max = arr[0];
    int iMax = 0;
    for(int i=1; i<size; i++){
        if(arr[i] > max){
            iMax = arr[i];
            iMax = i;
        }
    }
    return iMax;
}

_Bool rollDice(double chance){
    int value = (rand()%100)+1;
    if(value < 100 * chance){
        return 1;
    }
    return 0;
}

void mute(int *tabTour, unsigned int dim){
    int edge1 = rand() % dim;
    while(edge1 == 0){
        edge1 = rand() % dim;
    }
    int edge2 = rand() % dim;
    while(edge2 == edge1 || edge2 == 0){
        edge2 = rand() % dim;
    }
    int temp = tabTour[edge1];
    tabTour[edge1] = tabTour[edge2];
    tabTour[edge2] = temp;
}

int DPX(int **tabCoord, int *parent1, int *parent2, int *childBuffer, unsigned int size){
    int *extremities = (int *) malloc(2 * size * sizeof(int));
    if(extremities == NULL){
        fprintf(stderr, "ERROR : in DPX : error while allocating extremities\n");
        return -1;
    }
    int numberOfChunks = 0;


    //================== FIRST CHUNK RIGHT EXTREMITY =====================
    int nextNode;
    int neighbour;

    int rightExtremity = 0;
    int indexParent2 = indexOfNode(parent1[0], parent2, size);

    while( rightExtremity < size-1){
        nextNode = parent1[rightExtremity+1];
        neighbour = isNeighbour(indexParent2, nextNode, parent2, size);
        if(neighbour == -1){
            break;
        }
        rightExtremity++;
        indexParent2 = neighbour;
    }
    if(rightExtremity == size-1  ){
        memcpy(childBuffer, parent1, size * sizeof(int));
        //printf("same\n");
        free(extremities);
        return 0;
    }
    //================= FIRST CHUNK LEFT EXTREMITY =====================
    int leftExtremity = 0;
    indexParent2 = indexOfNode(parent1[0], parent2, size);

    if( (neighbour = isNeighbour(indexParent2, parent1[size-1], parent2, size) ) != -1 ){

        leftExtremity = size-1;
        indexParent2 = neighbour;

        while( leftExtremity > rightExtremity + 1 ){
            nextNode = parent1[leftExtremity-1];
            neighbour = isNeighbour(indexParent2, nextNode, parent2, size);
            if(neighbour == -1){
                break;
            }
            leftExtremity = leftExtremity-1;
            indexParent2 = neighbour;
        }
    }
    extremities[0] = leftExtremity;
    extremities[1] = rightExtremity;
    numberOfChunks++;

    //================= ALL OTHER CHUNKS ======================
    int end;
    if(leftExtremity == 0){
        end = size;
    }
    else{
        end = leftExtremity;
    }
    int i = rightExtremity+1;
    indexParent2 = indexOfNode(parent1[i], parent2, size);
    leftExtremity = i;
    
    while( i<end ){
        nextNode = parent1[i+1];
        neighbour = isNeighbour(indexParent2, nextNode, parent2, size);
        //printf("i = %d | node = %d | next node = %d | neighbour = %d\n", i, parent1[i], nextNode, neighbour);
        if(neighbour == -1){
            extremities[2*numberOfChunks] = leftExtremity;
            extremities[2*numberOfChunks+1] = i;
            leftExtremity = i+1;
            numberOfChunks++;
            i++;
            indexParent2 = indexOfNode(parent1[i], parent2, size);
        }
        else{
            i++;
            indexParent2 = neighbour;
        }
    }
    /*
    for(int chunk=0; chunk<numberOfChunks; chunk++){
        printf("%d %d\n", extremities[2*chunk], extremities[2*chunk+1]);
    }*/
    //=================== CREATING CHILD =======================================

    // COPYING FIRST CHUNK
    for(int i=0; i<=extremities[1]; i++){ 
        childBuffer[i] = parent1[i];
    }
    for(int i=size-1; i>=extremities[0]; i--){
        childBuffer[i] = parent1[i];
    }
    int lastChunkPlaced = 0;
    i = extremities[1]+1;
    extremities[0] = -1;
    extremities[1] = -1;

    // extremities containes the INDEXES of the end points of the chunks IN PARENT1
    // cPlaced is the count of how many chunks have been placed in child
    // iMinDist is the INDEX IN EXTREMITIES of the closest extremity to the last extremity that has been placed in child

    for(int cPlaced=1; cPlaced<numberOfChunks; cPlaced++){ // place every chunk

        int iMinDist = ((lastChunkPlaced+1)*2) % (numberOfChunks*2); // set iMinDist to be the first available chunk after the last one placed
        while(extremities[iMinDist] == -1){
            iMinDist = (iMinDist+1) % (numberOfChunks*2);
        }
        // k and iMinDist are indexes IN EXTREMITIES
        double minDist = distance(parent1[extremities[iMinDist]], childBuffer[i-1], tabCoord);
        int k = (iMinDist+1) % (2*numberOfChunks);
        while(k != ((lastChunkPlaced)*2) % (numberOfChunks*2)){
            if(extremities[k] != -1 ){
                double length = distance(parent1[extremities[k]], childBuffer[i-1], tabCoord);
                if(length < minDist){
                    //printf("last extre = %d | found imin = %d | length = %lf\n", childBuffer[i-1], k, length);
                    minDist = length;
                    iMinDist = k;
                }
            }
            k = (k+1) % (numberOfChunks*2);
        }
        // iMinDist is now the index in extremity that correspond to the closest extremity to the last point placed
        // the corresponding chunk now has to be copied in child
        if(iMinDist%2 == 0){
            for(int k=extremities[iMinDist]; k<=extremities[iMinDist+1]; k++){
                childBuffer[i] = parent1[k];
                i++;
            }
        }
        else{
            for(int k=extremities[iMinDist]; k>=extremities[iMinDist-1]; k--){
                childBuffer[i] = parent1[k];
                i++;
            }
        }
        // iteration updates
        lastChunkPlaced = iMinDist/2;
        extremities[lastChunkPlaced*2] = -1;
        extremities[(lastChunkPlaced*2)+1] = -1;
    }


    free(extremities);
    return 1;
}

//======================================================
//======================================================


int indexOfNode(int node, int *tabTour, unsigned int size){
    for(int i=0; i<size; i++){
        if(tabTour[i] == node){
            return i;
        }
    }
    return -1;
}

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

_Bool containsEdge(int A, int B, int *tabTour, unsigned int size){
    for(int i=1; i<size; i++){
        if( ( tabTour[i-1] == A && tabTour[i] == B ) || ( tabTour[i-1] == B && tabTour[i] == A ) ){
            return 1;
        }
    }
    return 0;
}/*
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
}

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
*/