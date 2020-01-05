#ifndef _TSP_PARSER_H
#define _TSP_PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include "globals.h"

// USED IN READKEYWORD AS RETURN VALUES
#define NAME_F 0b1
#define TYPE_F 0b10
#define DIMENSION_F 0b100
#define EDGE_WEIGHT_TYPE_F 0b1000
#define COMMENT_F 0b10000

// TSPLIB DATA STRUCTURE
typedef struct instance_s {
   char name[MAXNAMELENGTH]; // INSTANCE NAME
   char type[MAXNAMELENGTH]; // INSTANCE TYPE
   int dimension; // NUMBER OF VERTICES
   char edge_type[MAXNAMELENGTH];
   double length; // LENGTH OF CYCLE
   int **tabCoord; // COORDINATES MATRIX (X,Y,marque=0 pas vu, n° ordre)
   double **matDist; // HALF MATRIX OF EUCLIDIAN DISTANCES (calcul)
   int *tabTour; // ARRAY OF CITIES IN TOUR (dans l'ordre) (calcul)
} instance_t;

int parseTSPfile(FILE *fp, instance_t *instance, _Bool nz);

unsigned char specKeyword(const char str[]);

int initTSPinstance(instance_t *instance);

void trim(char str[]);

#endif