#ifndef _TSP_PARSER_H
#define _TSP_PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include "globals.h"

/**
 * \file    TSP_parser.h
 * \brief   This parses a tsp file, entry point is parseTSPfile, returns an instance_s structure
 * \author  BERNARD O.
 * \date    december 2019
 */

/// \def    Used as return value to identify the keywords NAME
#define NAME_F 0b1
/// \def    Used as return value to identify the keywords TYPE
#define TYPE_F 0b10
/// \def    Used as return value to identify the keywords DIMENSION
#define DIMENSION_F 0b100
/// \def    Used as return value to identify the keywords EDGE_WEIGHT_TYPE
#define EDGE_WEIGHT_TYPE_F 0b1000
/// \def    Used as return value to identify the keywords COMMENT
#define COMMENT_F 0b10000


/** 
 * \struct  instance_t TSP_parser.h
 * \brief   An instance of a tsp problem
 * 
 * name        :  the name of the instance
 * type        :  the type of the instance, must be tsp in our case
 * dimension   :  the dimension of the instance, must be a strictly positive integer
 * edge_type   :  the type of edge in the tsp graph, must be EUC_2D in our case
 * tabCoord    :  a two dimensionnal array containing the x and y value of the i-th node 
 * matDist     :  a two dimensionnal array containing the distance between the i-th and j-th nodes
 * tabTour     :  an array containing the sequence of nodes visited in a given tour
 */
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

/**
 * \brief   Parses a tsp file, returns the result in an instance_t structure. IT IS VERY STRICT ON THE FILE FORMAT
 * 
 * \param   fp          :   a file pointer to the tsp file
 * \param   instance    :   buffer to wich the result are written
 * \param   nz          :   boolean value : 0 if the (0,0) node has to be is added, 0 if not
 * 
 * \return  a negavive value if an error has occured, 0 otherwise
 */
int parseTSPfile(FILE *fp, instance_t *instance, _Bool nz);

/**
 * \brief   Identifies if a string is one of the tsp file keywords
 * 
 * \param   str         :  the string to be checked
 *
 * \return  One of the keyword flag defined in TSP_parser.h or 0 if str did not match anything
 */
unsigned char specKeyword(const char str[]);

/**
 * \brief   Initialize the values of an instance_t structure
 * 
 * \param   instance    :   a pointer to the instance
 */
void initTSPinstance(instance_t *instance);

/**
 * \brief   Removes heading and trailing spaces of a string.
 * 
 * \param   str         :   the string to be trimmed
 */
void trim(char str[]);


/**
 * \brief   Prints the content of a tsp instance in a file given as parameter
 * 
 * \param   fp          :   A file pointer
 * \param   instance    :   An instance_t structure
 */
void printTSPinstance(FILE *fp, instance_t instance);

#endif