#ifndef _TSP_PARSER_Q_H
#define _TSP_PARSER_Q_H

#include <stdio.h>

#define MAXNAMELENGTH 32

// USED IN READKEYWORD AS RETURN VALUES
#define NAME_F 0b1
#define TYPE_F 0b10
#define DIMENSION_F 0b100
#define EDGE_WEIGHT_TYPE_F 0b1000
#define COMMENT_F 0
#define EOF_REACHED_F -1
#define NODE_COORD_SECTION_F 0b10000

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

int parseFile(FILE *fp, instance_t *instance);
int parseData(instance_t *instace);
int parseDataLine(char line[], int *nLineBuff, double *xBuff, double *yBuff);
int spaceDivide(char str[]);

int parseSpecs(instance_t *instance);
int readName(char dest[], char src[]);
int readType(char dest[], char src[]);
int readDimension(char src[]);
int readEdgeType(char dest[], char src[]);
int readComment(char comment[]);
int colonDivide(char str[]);

int tokenize(FILE *fp);
void trim(char str[]);
int specKeyword(const char str[]);

#endif