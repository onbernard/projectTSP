#ifndef _NEAREST_NEIGHBOUR_H
#define _NEAREST_NEIGHBOUR_H

#include "TSP_parser.h"

/**
 * \file    nearest_neighbour.h
 * \brief   HEADER - Implements the nearest neighbour algorithm to solve a tsp problem
 * \author  BERNARD O.
 * \date    december 2019
 */


/**
 * \brief   Implements the nearest neighbour algorithm to solve the tsp problem represented by instance
 * 
 * \param   instance        :   A pointer to an instance_t struct representing the tsp problem
 * \param   tourBuffer      :   A pointer to the array where to store the best tour
 * 
 * \return  A double representing the length of the best tour that was found
 */
double nearestNeighbourSolver(instance_t *instance, int *tourBuffer);

/**
 * \brief   Finds the city number that is closest to the city with index cityA and whose value in freeCities is 1
 * 
 * \param   cityA           :   The number of the city
 * \param   tabCoord        :   A pointer to the two dimensionnal array with the cities coordinates
 * \param   freeCities      :   A pointer to the array with a boolean value for each city: 1 if free, 0 if not
 * \param   size            :   The first order size of these arrays
 * 
 * \return  The index of the closest free neighbour
 */
int findNearestNeighbour(int cityA, int **tabCoord, int *freeCities, unsigned int size);

/**
 * \brief   Computes the distance between cityA and cityB using coordinates in tabCoord
 * 
 * \param   cityA           :   The number of the first city
 * \param   cityB           :   The number of the second city
 * \param   tabCoord        :   A pointer to the two dimensionnal array with the cities coordinates                                   
 * 
 * \return  The distance between the cities as a double
 */
double distance(int cityA, int cityB, int **tabCoord);

#endif