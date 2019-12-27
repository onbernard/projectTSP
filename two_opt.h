#ifndef _TWO_OPT_H
#define _TWO_OPT_H

#include "TSP_parser_Q.h"

void twoOpt(instance_t *instance, int *tabTour);

double absF(double x);

_Bool isCrossing(int **tabCoord, int A, int B, int C, int D);

void L(double mat[2][3], int l1, int l2, double m);

void M(double mat[2][3], int l, double m);

void P(double mat[2][3], int l1, int l2);

void printSyst(double mat[2][3]);

#endif