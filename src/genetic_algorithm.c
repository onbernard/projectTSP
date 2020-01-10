#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "genetic_algorithm.h"

#include "brute_force.h"
#include "globals.h"
#include "TSP_parser.h"
#include "random_walk.h"
#include "nearest_neighbour.h"
#include "tools.h"

/**
 * \file    genetic_algorithm.c
 * \brief   SOURCE - Implements the genetic algorithm to solve a tsp problem
 * \author  BERNARD O.
 * \date    december 2019
 */



/// Implements the genetic algorithm on the tsp problem represented by instance
/// The numbers of breeding is generated randomly each generation
/// All the complexity and spagetti is in DPX
/// CALLS ABORT ON ALLOCATION ERRORS
double geneticAlgorithmSolver(instance_t *instance, int *tourBuffer, int nSpecimens, int nGenerations, double mutationRate){
    if(verbose){
        fprintf(logfileP, "========== IN GA ==========\n");
    }

    int dim = instance->dimension;
    time_t t;
    srand((unsigned) time(&t));

    double worseLength = -1; // the one that will be replaced
    int worseIndex = -1;

    // ========================== INITIALIZATION OF POPULATION ====================================================================

    double *lengthsArr = (double *) malloc(nSpecimens * sizeof(double)); // POPULATION LENGTHS ARRAY
    if(lengthsArr == NULL){ // dealoc
        fprintf(stderr, "ERROR : in geneticAlgorithmSolver : error while allocating lengthsArr\nAborting...\n");
        abort();
    }
    int **population = (int **) malloc(nSpecimens * sizeof(int *)); // POPULATION ARRAY
    if(population == NULL){ // dealoc
        fprintf(stderr, "ERROR : in geneticAlgorithmSolver : error while allocating population\nAborting...\n");
        abort();
    }
    for(int i=0; i<nSpecimens; i++){
        population[i] = (int *) malloc(dim * sizeof(int)); // AN INDIVIDUAL
        if(population[i] == NULL){ // dealoc
            fprintf(stderr, "ERROR : in geneticAlgorithmSolver : error while allocating population[%d]\nAborting...\n", i);
            abort();
        }
        randomWalkSolver(instance, population[i]); // INITIALIZE IT RANDOMLY
        lengthsArr[i] = tourLength(population[i], instance->tabCoord, dim); // FILL POPULATION LENGTHS ARRAY
        if(worseIndex == -1 || lengthsArr[i] > worseLength){ // UPDATE WORSE ONE
            worseIndex = i;
            worseLength = lengthsArr[i];
        }
    }

    int *childBuffer = (int *) malloc(dim * sizeof(int)); // WHERE BABIES ARE MADE
    if(childBuffer == NULL){ // dealoc
        fprintf(stderr, "ERROR : in geneticAlgorithmSolver : error while allocating childBuffer\nAborting...\n");
        abort();
    }
    // ===========================================================================================================================
    
    // TODO PRINT BEST

    // =========================== ITERATE OVER GENERATIONS =========================================================

    for(int i=0; i<nGenerations; i++){
        int numberOfCrossBreeding = rand() % (nSpecimens/2); // NOT EVERYONE GETS LAID
        if(verbose){
            fprintf(logfileP, "Generation %d, there will be %d breedings\n", i, numberOfCrossBreeding);
            fprintf(logfileP, "Worse specimen has length %lf:\n", worseLength);
        }
        for(int k=0; k<numberOfCrossBreeding; k++){ // BREEDING SEASON!!!!
            int parent1 = rand() % nSpecimens; // YOU GET A BABY
            int parent2 = rand() % nSpecimens; // YOU GET A BABY
            while(parent2 == parent1){
                parent2 = rand() % nSpecimens;
            }
            if( DPX(instance->tabCoord, population[parent1], population[parent2], childBuffer, dim) < 0 ){ // TODO
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

    if(verbose){
        fprintf(logfileP, "========== OUT GA ==========\n");
    }
    return bestLength;
}

/// Returns the index of the minimum integer value in arr
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

/// Returns the index of the maximum integer value in arr
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

/// Returns 1 in chance*100 % of cases
_Bool rollDice(double chance){
    int value = (rand()%100)+1;
    if(value < 100 * chance){
        return 1;
    }
    return 0;
}

/// Swap randomly two cities in tour array
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

/// CAUTION SPAGETTI CODE AHEAD
/// First city of child is always the first city of parent1 to keep consistency with the rest of the code
int DPX(int **tabCoord, int *parent1, int *parent2, int *childBuffer, unsigned int size){
    int *extremities = (int *) malloc(2 * size * sizeof(int)); // dealoc
    if(extremities == NULL){
        fprintf(stderr, "ERROR : in DPX : error while allocating extremities\nAborting...\n");
        abort();
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

/// Returns the index of city node in a tour array
int indexOfNode(int node, int *tabTour, unsigned int size){
    for(int i=0; i<size; i++){
        if(tabTour[i] == node){
            return i;
        }
    }
    return -1;
}


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