#ifndef _TSP_LOADER_H
#define _TSP_LOADER_H

#include "globals.h"

// USED IN READKEYWORD AS RETURN VALUES
#define NAME_F 0b1
#define TYPE_F 0b10
#define DIMENSION_F 0b100
#define EDGE_WEIGHT_TYPE_F 0b1000
#define COMMENT_F 0b10000
#define EOF_REACHED_F 0b100000
#define NODE_COORD_SECTION_F 0b1000000

// USED IN READTYPE AS RETURN VALUES
#define TSP_F 1
#define HCP_F 2
#define ATSP_F 3
#define SOP_F 4
#define CVRP_F 5

// USED IN READEDGEWEIGHTTYPE AS RETURN VALUES
#define EXPLICIT_F 6
#define EUC_2D_F 7
#define EUC_3D_F 8
#define MAX_2D_F 9
#define MAX_3D_F 10
#define MAN_2D_F 11
#define MAN_3D_F 12
#define CEIL_F 13
#define GEO_F 14
#define ATT_F 15
#define XRAY1_F 16
#define XRAY2_F 17
#define SPECIAL_F 18

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


// return 0 if something other than a letter or _ is read
// NAME_F name, TYPE_F type, DIMENSION_F dimension
// EDGE_WEIGHT_TYPE_F edge_weight_type, COMMENT comment
// EOF_REACHED_F eof
unsigned char readSpecKeyword(FILE *fp);

// read characters from fp until either 'eof', '\n' or ':' is reached
// return 0 if 'eof' or '\n' reached
// 1 if ':' reached
int readSeparator(FILE *fp);

// read characters from fp until eof, '\n' or MAXNAMELENGTH -1 characters is read
// return 1 if a name is read and '\n' is encoutered
// 0 if '\n' is encountered but no characters has been read
// 0 if eof is reached or if MAXNAMELENGTH -1 has been read
int readName(FILE *fp, char buffer[]);

// read from fp
// return 0 if more than 4 characters are read
// return 0 if eof 
int readType(FILE *fp);

#endif