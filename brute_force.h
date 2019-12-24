#ifndef _BRUTE_FORCE_H
#define _BRUTE_FORCE_H

#include "TSP_parser_Q.h"

void bruteForceMatrix(instance_t *instance, int *tourBuffer);

void bruteForce(instance_t *instance, int *tourBuffer);

void fillMatDist(double **matDist, int **tabCoord, unsigned int size);

double tourLength(int *tabTour, int **tabCoord, unsigned int size);

double tourLengthMatrix(int *tabTour, int **tabCoord, double **matDist, unsigned int size);

int next(int *tabTour, unsigned int size);

int compare(const void *a, const void *b);

void swap(int *tab, int i, int j);


#endif