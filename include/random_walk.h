#ifndef _RANDOM_WALK_H
#define _RANDOM_WALK_H

#include "TSP_parser.h"

/**
 * \file    random_walk.h
 * \brief   HEADER - Implements the random_walk algorithm to solve a tsp problem
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
double randomWalkSolver(instance_t *instance, int *tourBuffer);

#endif