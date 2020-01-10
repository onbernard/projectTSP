#ifndef _GENETIC_ALGORITHM_H
#define _GENETIC_ALGORITHM_H

#include "TSP_parser.h"

/**
 * \file    genetic_algorithm.h
 * \brief   HEADER - Implements the genetic algorithm to solve a tsp problem
 * \author  BERNARD O.
 * \date    december 2019
 */


/**
 * \brief   Implements the genetic algorithm to solve the tsp problem represented by instance
 * 
 * \param   instance        :   A pointer to an instance_t struct representing a tsp problem
 * \param   tourBuffer      :   A pointer to the array where to store the best tour
 * \param   nSpecimens      :   An integer representing the number of tours that will comprise the genetic pool
 * \param   nGenerations    :   An integer representing the number of generations to iterate over
 * \param   mutationRate    :   A double value representing the share of younglings that will be mutated
 * 
 * \return  A double representing the length of the best tour that was found
 */
double geneticAlgorithmSolver(instance_t *instance, int *tourBuffer, int nSpecimens, int nGenerations, double mutationRate);


/**
 * \brief   Implements the DPX crossing between tours parent1 and parent2
 * 
 * \param   tabCoord        :   A two dimensionnal array containing the coordinates of the cities
 * \param   parent1         :   The tour array of parent1
 * \param   parent2         :   The tour array of parent2
 * \param   childBuffer     :   A buffer to wich the tour array child will be placed
 * \param   size            :   The first order size of these arrays
 * 
 * \return  0 if parent1 and parent2 are identical and a child was not generated, -1 if an error has occured, 1 otherwise
 */
int DPX(int **tabCoord, int *parent1, int *parent2, int *childBuffer, unsigned int size);


/**
 * \brief   Swap randomly two cities in tour array
 * 
 * \param   tabTour        :   A tour array
 * \param   dim            :   The size of the tour array
 */
void mute(int *tabTour, unsigned int dim);


/**
 * \brief   Returns the index of the minimum integer value in arr
 * 
 * \param   arr             :   A tour array
 * \param   size            :   The size of the tour array
 * 
 * \return The index of the minimum integer value in arr
 */
int iMinInArray(double *arr, unsigned int size);


/**
 * \brief   Returns the index of the maximum integer value in arr
 * 
 * \param   arr             :   A tour array
 * \param   size            :   The size of the tour array
 * 
 * \return The index of the maximum integer value in arr
 */
int iMaxInArray(double *arr, unsigned int size);


/**
 * \brief   Returns 1 in chance*100 % of cases
 * 
 * \param   chance          :   The chance of getting a 1
 * 
 * \return 1 in chance*100 % of cases
 */
_Bool rollDice(double chance);


/**
 * \brief   Returns the index of city node in a tour array
 * 
 * \param   node            :   The number of the city
 * \param   tabTour         :   The tour array where to search
 * \param   size            :   The size of the tour array
 * 
 * \return 1 in chance*100 % of cases
 */
int indexOfNode(int node, int *tabTour, unsigned int size);


/**
 * \brief   Returns 1 if city node is to the left or right of index index in array tour
 * 
 * \param   index           :   The index of the position
 * \param   node            :   The number of the city
 * \param   tabTour         :   The tour array where to search
 * \param   size            :   The size of the tour array
 * 
 * \return 1 in chance*100 % of cases
 */
int isNeighbour(int index, int node, int *tabTour, unsigned int size);


/*
void test1();

void test2();
*/
#endif