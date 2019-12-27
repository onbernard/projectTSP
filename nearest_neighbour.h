#ifndef _NEAREST_NEIGHBOUR_H
#define _NEAREST_NEIGHBOUR_H

#include "TSP_parser_Q.h"

double nearestNeighbourSolver(instance_t *instance, int *tourBuffer);

int findNearestNeighbour(int node, int **tabCoord, unsigned int size, int *freeNodes);

int nearestToZero(int **tabCoord, unsigned int size);

double distanceFromZero(int node, int **tabCoord);

double distance(int pointA, int pointB, int **tabCoord);

#endif