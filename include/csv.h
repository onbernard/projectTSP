#ifndef _CSV_H
#define _CSV_H

#include <stdio.h>

#include "globals.h"

void printRow(FILE *fp, char name[MAXNAMELENGTH], char method[MAXNAMELENGTH], double length, double time, int *tabTour, unsigned int size);


#endif