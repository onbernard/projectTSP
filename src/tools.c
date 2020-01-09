#include <stdio.h>
#include <stdlib.h>
#include "tools.h"

#include "globals.h"

// distance

// tourlength

void printTour(int *tabTour, char separator, unsigned int size){
    for(int i=0; i<size; i++){
        fprintf(logfileP, "%d%c", tabTour[i], separator);
    }
    fprintf(logfileP, "\n");
}