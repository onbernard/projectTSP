#ifndef _TOUR_PARSER_H
#define _TOUR_PARSER_H

#include "TSP_parser.h"

int parseTOURfile(FILE *fp, instance_t *instance, _Bool nz);

_Bool isInArr(int *arr, int value, unsigned int size);

int initTOURinstance(instance_t *instance);

#endif