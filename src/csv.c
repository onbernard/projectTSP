#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"

#include "globals.h"

// instance methode longueur temps liste
void printRow(FILE *fp, char name[MAXNAMELENGTH], char method[MAXNAMELENGTH], double length, double time, int *tabTour, unsigned int size){
    fprintf(fp, "\"%s\";\"%s\";\"%lf\";\"%lf\";\"[%d", name, method, length, time, tabTour[0]);
    for(int i=1; i<size; i++){
        fprintf(fp, ",%d", tabTour[i]);
    }
    fprintf(fp, "]\"\n");
}