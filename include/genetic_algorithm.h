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

int DPX(int **tabCoord, int *parent1, int *parent2, int *childBuffer, unsigned int size);

void mute(int *tabTour, unsigned int dim);

int iMinInArray(double *arr, unsigned int size);

int iMaxInArray(double *arr, unsigned int size);

_Bool rollDice(double chance);

int indexOfNode(int node, int *tabTour, unsigned int size);

int isNeighbour(int index, int node, int *tabTour, unsigned int size);

_Bool containsEdge(int A, int B, int *tabTour, unsigned int size);

/*
void test1();

void test2();
*/
#endif