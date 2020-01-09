#ifndef _TWO_OPT_H
#define _TWO_OPT_H

#include "TSP_parser.h"

/**
 * \file    two_opt.c
 * \brief   HEADER - Implements the 2-opt algorithm to solve a tsp problem
 * \author  BERNARD O.
 * \date    december 2019
 */


/**
 * \brief   Implements the 2-opt algorithm to solve the tsp problem represented by instance
 * 
 * \param   instance        :   A pointer to an instance_t struct representing the tsp problem
 * \param   tourBuffer      :   A pointer to the array where to store the best tour
 * 
 * \return  A double representing the length of the best tour that was found
 */
double twoOptSolver(instance_t *instance, int *tourBuffer);


/**
 * \brief   Iterates over every segment and following segments and swaps the end values when they crosses
 * 
 * \param   instance        :   A pointer to an instance_t struct representing a tsp problem
 * \param   tabTour         :   A pointer to the integer tour array to decross
 * 
 * \return  A integer representing the number of segments that were decrossed
 */
int doOneDecrossPass(instance_t *instance, int *tabTour);


/**
 * \brief   Prints informations about every crossings in tabTour and returns the number of them
 * 
 * \param   instance        :   A pointer to an instance_t struct representing a tsp problem
 * \param   tabTour         :   A pointer to an integer tour array
 * 
 * \return  A integer representing the number of crossing segments
 */
int numberOfCrossing(instance_t *instance, int *tabTour);


/**
 * \brief   Returns the absolute value of a double
 * 
 * \param   x               :  A double value
 * 
 * \return  The absolute value of x
 */
double absF(double x);


/**
 * \brief   Returns 1 if segments of city A to city B and city C to city D crosses
 * 
 * \param   tabCoord        :   A pointer to a two dimensionnal array containing the cities coordinates
 * \param   A               :   Number of city A
 * \param   B               :   Number of city B
 * \param   C               :   Number of city C
 * \param   D               :   Number of city D
 * 
 * \return  1 if they are crossing, O if not
 */
_Bool isCrossing(int **tabCoord, int A, int B, int C, int D);


/**
 * \brief   Add line l1 times m to line l2 in 2 by 3 dimensionnal double array
 * 
 * \param   mat             :   A 2 by 3 dimensionnal double array
 * \param   l1              :   Number of line that will be modified
 * \param   l2              :   Number of line that will be added
 * \param   m               :   Factor by wich l2 will be multiplied
 */
void L(double mat[2][3], int l1, int l2, double m);


/**
 * \brief   Multiply line l of 2 by 3 dimensionnal double array by m
 * 
 * \param   mat             :   A 2 by 3 dimensionnal double array
 * \param   l               :   Number of line that will be modified
 * \param   m               :   Factor by wich l will be multiplied
 */
void M(double mat[2][3], int l, double m);


/**
 * \brief   Permutes lines l1 et l2 of 2 by 3 dimensionnal double array
 * 
 * \param   mat             :   A 2 by 3 dimensionnal double array
 * \param   l1              :   Number of first line
 * \param   l2              :   Number of second line
 */
void P(double mat[2][3], int l1, int l2);


/**
 * \brief   Prints a 2 by 3 dimensionnal double array
 * 
 * \param   mat             :   A 2 by 3 dimensionnal double array
 */
void printSyst(double mat[2][3]);

#endif