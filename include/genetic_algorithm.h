#ifndef _GENETIC_ALGORITHM_H
#define _GENETIC_ALGORITHM_H

#include "TSP_parser_Q.h"

double geneticAlgorithmSolver(instance_t *instance, int *tourBuffer, int nSpecimens, int nGenerations, double mutationRate);

int DPX(int **tabCoord, int *parent1, int *parent2, int *childBuffer, unsigned int size);

void mute(int *tabTour, unsigned int dim);

int iMinInArray(double *arr, unsigned int size);

int iMaxInArray(double *arr, unsigned int size);

_Bool rollDice(double chance);

int indexOfNode(int node, int *tabTour, unsigned int size);

int isNeighbour(int index, int node, int *tabTour, unsigned int size);

_Bool containsEdge(int A, int B, int *tabTour, unsigned int size);

/*
void test1();

void test2();
*/
#endif