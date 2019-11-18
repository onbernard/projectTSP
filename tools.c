#include <stdio.h>
#include <stdlib.h>
#include "tools.h"

#include "globals.h"

int stoui(const char *str){
    char *c = str;
    int outp = 0;
    while(c != '\0'){
        if(*c >= '0' && *c <= '9'){
            outp *= 10;
            outp += *c -'0';
        }
        else{
            return -1;
        }
        c++;
    }
    return outp;
}

double stod(const char *str){
    char *c = str;
    double wholePart = 0;
    double decimalPart = 0;
    while(c != '\0'){
        if(*c >= '0' && *c <= '9'){
            outp *= 10;
            outp += *c -'0';
        }
        else{
            return -1;
        }
        c++;
    }
    return outp;
}