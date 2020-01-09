#ifndef _TOUR_PARSER_H
#define _TOUR_PARSER_H

#include "TSP_parser.h"

/**
 * \file    TOUR_parser.h
 * \brief   HEADER - Parser for a tour file
 * \author  BERNARD O.
 * \date    december 2019
 */


/**
 * \brief   Parses a tour file, returns the result in an instance_t structure. IT IS VERY STRICT ON THE FILE FORMAT
 * 
 * \param   fp          :   a file pointer to the tour file
 * \param   instance    :   buffer to wich the result are written
 * \param   nz          :   boolean value : 0 if the (0,0) node has to be is added, 0 if not
 * 
 * \return  a negavive value if an error has occured, 0 otherwise
 */
int parseTOURfile(FILE *fp, instance_t *instance, _Bool nz);


/**
 * \brief   Returns 1 if value is in array
 * 
 * \param   arr         :   An integer array
 * \param   value       :   An integer value that will be searched for
 * \param   size        :   The size of the array
 * 
 * \return  1 if value is in array, 0 if not
 */
_Bool isInArr(int *arr, int value, unsigned int size);

/**
 * \brief   Initializes an instance_t structure for a tour file
 * 
 * \param   instance         :   A pointer to an instance_t structure
 */
void initTOURinstance(instance_t *instance);

/**
 * \brief   Prints the content of a tour instance in a file given as parameter
 * 
 * \param   fp          :   A file pointer
 * \param   instance    :   An instance_t structure
 */
void printTOURinstance(FILE *fp, instance_t instance);

#endif