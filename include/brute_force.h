#ifndef _BRUTE_FORCE_H
#define _BRUTE_FORCE_H

#include "TSP_parser.h"

/**
 * \file    brute_force.h
 * \brief   HEADER - Implements the brute force methode either with a distance matrix or not. 
 * \author  BERNARD O.
 * \date    december 2019
 */



/**
 * \brief   Implements the brute force algorithm using a distance matrix
 * 
 * \param   instance        :   A pointer to an instance_t struct representing the tsp problem
 * \param   tourBuffer      :   A pointer to the array where to store the best tour
 * 
 * \return  A double representing the length of the best tour that was found
 */
double bruteForceMatrix(instance_t *instance, int *tourBuffer);

/**
 * \brief   Implements the brute force algorithm calculating every distance on the fly
 * 
 * \param   instance        :   A pointer to an instance_t struct representing the tsp problem
 * \param   tourBuffer      :   A pointer to the array where to store the best tour
 * 
 * \return  A double representing length of the best tour that was found
 */
double bruteForce(instance_t *instance, int *tourBuffer);

/**
 * \brief   Fills the array pointed by matDist with the distances between cities with coordinates in tabCoord
 * 
 * \param   matDist         :   A pointer to a two dimensionnal array to where distances will be written
 * \param   tabCoord        :   A pointer to a two dimensionnal array containing the coordinates of the cities
 * \param   size            :   The first order size of these arrays
 */
void fillMatDist(double **matDist, int **tabCoord, unsigned int size);

/**
 * \brief   Computes the length of the tour pointed to by tabTour using coordinates in tabCoord
 * 
 * \param   tabTour         :   A pointer to the array containing the permutation of cities representing the tour
 * \param   tabCoord        :   A pointer to the two dimensionnal array containing the coordinates of the cities
 * \param   size            :   The first order size of these arrays
 * 
 * \return  A double representing the length of the tour
 */
double tourLength(int *tabTour, int **tabCoord, unsigned int size);


/**
 * \brief   Computes the length of the tour pointed to by tabTour using the distance matrix matDist
 * 
 * \param   tabTour         :   A pointer to the array containing the permutation of cities representing the tour
 * \param   matDist         :   A pointer to the two dimensionnal array containing the distances between the cities
 * \param   size            :   The first order size of these arrays
 * 
 * \return  A double representing the length of the tour
 */
double tourLengthMatrix(int *tabTour , double **matDist, unsigned int size);

/**
 * \brief   Generates the next permutation of integer sequence in lexicographic order. Tabtour is overwritten if there is a next one after it.
 * 
 * \param   tabTour         :   A pointer to the array containing the permutation of cities
 * \param   size            :   The size of the tabTour array
 * 
 * \return  A boolean value. 1 if there is no next permutation, 0 otherwise and tabTour is overwritten
 */
int next(int *tabTour, unsigned int size);

/**
 * \brief   For use in next to sort the array, compares two integers pointed by a and b. Simple >/<
 */
int compare(const void *a, const void *b);

/**
 * \brief   Swaps two values in the tab array
 */
void swap(int *tab, int i, int j);


#endif