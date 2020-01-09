#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TSP_parser.h"

#include "globals.h"

/**
 * \file    TSP_parser.c
 * \brief   SOURCE - Parser for a tsp file
 * \author  BERNARD O.
 * \date    december 2019
 */

/// Parses a tsp file.
/// It allocates instance->tabTour and the two dimensionnal array instance->tabCoord
/// CALLS ABORT ON ALLOCATION ERROR
/// If nz is 0 then a point with coordinates (0,0) is automatically placed on the first index and dimension is incremented by one
/// Right now these are accepted:
///     ::: firsthalf ::::: second half :::  fzefzeafeza
/// Only EUC_2D edges type and TSP file type are accepted
/// Integers must be strictly greater than 0
/// A city number must be equal to its position
int parseTSPfile(FILE *fp, instance_t *instance, _Bool nz){
    char buffer[512]; // IF YOU CAN TWEET IN 512 CHARACTERS YOU CAN WRITE A LINE IN THAT SPACE
    char *err = buffer;
    unsigned char specs_FLAGS = 0; // CONTAINS BITWISE FLAGS FOR THE SPECIFICATION KEYWORDS
    unsigned int nLine = 0;

    while( specs_FLAGS != (NAME_F|TYPE_F|DIMENSION_F|EDGE_WEIGHT_TYPE_F) ){

        // =============================== GET LINE =================================

        err = fgets(buffer, 512, fp);
        if(err == NULL){
            fprintf(stderr, "TSP FILE FORMAT ERROR : eof reached at line %d\n", nLine);
            return -1;
        }
        int endL =strcspn(buffer, "\n");
        buffer[endL] = 0; // to get rid of the trailing \n
        nLine++;

        // ----------------- DIVIDE IT  UP -----------------
        char *firstHalf = strtok(buffer, ":");
        if(firstHalf == NULL){
            fprintf(stderr, "TSP FILE FORMAT ERROR : empty line\n");
            return -1;
        }
        trim(firstHalf);

        char *secondHalf = strtok(NULL, ":");
        if(secondHalf == NULL){
            fprintf(stderr, "TSP FILE FORMAT ERROR : no colon separator in line %d\n", nLine);
            return -1;
        }
        trim(secondHalf);


        if(strlen(firstHalf) == 0){
            fprintf(stderr, "TSP FILE FORMAT ERROR : empty first half of line %d\n", nLine);
            return -1;
        }
        if(strlen(secondHalf) == 0){
            fprintf(stderr,"TSP FILE FORMAT ERROR : empty second half of line %d\n", nLine);
            return -1;
        }
        // ======================== LINE TREATMENT =============================
        unsigned char keyword = specKeyword(firstHalf);
        if( (specs_FLAGS & keyword) != 0){
            fprintf(stderr, "TSP FILE FORMAT ERROR : %s already specified\n", firstHalf);
            return -1;
        }
         // =========================== SWITCH =========================
        switch(keyword){
            case NAME_F:
                strncpy(instance->name, secondHalf, MAXNAMELENGTH-1);
                specs_FLAGS += NAME_F;
                break;
            case TYPE_F:
                if( strcmp(secondHalf, "TSP") !=0 ){
                    fprintf(stderr, "TSP FILE FORMAT ERROR : type has to be TSP\n");
                    return -1;
                }
                strcpy(instance->type, "TSP");
                specs_FLAGS += TYPE_F;
                break;
            case DIMENSION_F:
                if( atoi(secondHalf) > 0 ){
                    instance->dimension = atoi(secondHalf) + !nz;
                }
                else{
                    fprintf(stderr, "TOUR FILE FORMAT ERROR : dimension %s is not valid\n", secondHalf);
                    return -1;
                }
                specs_FLAGS += DIMENSION_F;
                break;
            case EDGE_WEIGHT_TYPE_F:
                if( strcmp(secondHalf, "EUC_2D") != 0 ){
                    fprintf(stderr, "TSP FILE FORMAT ERROR : edge weight type has to be EUC_2D\n");
                    return -1;
                }
                strcpy(instance->edge_type, "EUC_2D");
                specs_FLAGS += EDGE_WEIGHT_TYPE_F;
                break;
            case COMMENT_F:
                printf("Comment : %s\n", secondHalf);
                break;
            default:
                fprintf(stderr, "TOUR FILE FORMAT ERROR : unexpected keyword : %s\n", firstHalf);
                return -1;
        } // switch
    } // specs keyword loop

    // ============================= DATA PART ============================
    err = fgets(buffer, MAXNAMELENGTH, fp);
    if(err == NULL){
        fprintf(stderr, "TOUR FILE FORMAT ERROR : eof reached at line %d\n", nLine);
        return -1;
    }
    nLine++;

    buffer[strcspn(buffer, "\n")] = 0;
    trim(buffer);
    if( strcmp(buffer, "NODE_COORD_SECTION") != 0 ){
        fprintf(stderr, "TSP FILE FORMAT ERROR : unexpected keyword : %s\n", buffer);
        return -1;
    }

    // TABTOUR AND TABCOORD INSTANCIATION
    instance->tabTour = (int *) malloc( instance->dimension * sizeof(int));
    if(instance->tabTour == NULL){
        fprintf(stderr, "ERROR : in parseTSPfile : error while allocating tabTour\nAborting...\n");
        abort();
    }
    instance->tabCoord = (int **) malloc( instance->dimension * sizeof(int *) );
    if(instance->tabCoord == NULL){
        fprintf(stderr, "ERROR : in parseTSPfile : error while allocating tabCoord\n, Aborting...\n");
        abort();
    }
    for(int i=0; i<instance->dimension; i++){
        (instance->tabCoord)[i] = (int *) malloc(2 * sizeof(int));
        if( (instance->tabCoord)[i] == NULL ){
            fprintf(stderr, "ERROR : in parseTSPfile : error while allocating tabCoord[%d]\n, Aborting...\n", i);
            abort();
        }
    }

    // ========================== READ DATA =======================
    int i;
    int nCity = 1;;
    if(nz){
        i = 0;
    }
    else{
        (instance->tabCoord)[0][0] = 0;
        (instance->tabCoord)[0][1] = 0;
        i = 1;
    }
    while(i<instance->dimension){
        err = fgets(buffer, 512, fp);
        if(err == NULL){
            fprintf(stderr, "TSP FILE FORMAT ERROR : eof reached at line %d\n", nLine);
            return -1;
        }
        nLine++;
        buffer[strcspn(buffer, "\n")] = 0;
        char *svptr; // HERE OFFICER
        char *firstPart = strtok_r(buffer, " ", &svptr);
        //printf("officer %s\n", svptr);
        if(firstPart == NULL){
            fprintf(stderr, "TSP FILE FORMAT ERROR : empty line at line %d\n", nLine);
            return -1;
        }
        char *secondPart = strtok_r(NULL, " ", &svptr);
        //printf("officer %s\n", svptr);
        if(secondPart == NULL){
            fprintf(stderr, "TSP FILE FORMAT ERROR : invalid line format at line %d\n", nLine);
            return -1;
        }
        char *thirdPart = strtok_r(NULL, " ", &svptr);
        //printf("officer %s\n", svptr);
        if(thirdPart == NULL){
            fprintf(stderr, "TSP¨FILE FORMAT ERROR : invalid line format at line %d\n", nLine);
            return -1;
        }

        int n = atoi(firstPart);
        if( n <= 0 || n != nCity ){
            fprintf(stderr, "TSP FILE FORMAT ERROR : invalid city number %s at line %d\n", firstPart, nLine);
            return -1;
        }
        int x = atoi(secondPart);
        if( x <= 0 ){
            fprintf(stderr, "TSP FILE FORMAT ERROR : invalid x coordinate %s at line %d\n", secondPart, nLine);
            return -1;
        }
        int y = atoi(thirdPart);
        if( y <= 0 ){
            fprintf(stderr, "TSP FILE FORMAT ERROR : invalid y coordinate %s at line %d\n", thirdPart, nLine);
            return -1;
        }

        instance->tabCoord[i][0] = x;
        instance->tabCoord[i][1] = y;
        i++;
        nCity++;

    } // while data part
    err = fgets(buffer, 512, fp);
    if(err == NULL){
        fprintf(stderr, "TSP FILE FORMAT ERROR : eof reached at line %d\n", nLine);
        return -1;
    }
    nLine++;
    buffer[strcspn(buffer, "\n")] = 0;
    trim(buffer);
    if( strcmp(buffer, "EOF") != 0 ){
        fprintf(stderr, "TSP FILE FORMAT ERROR : expected EOF at line %d but got %s\n", nLine, buffer);
        return -1;
    }

    return 0;
}

/// Returns the flag corresponding to a keyword if str matches one
/// If not it returns 0
unsigned char specKeyword(const char str[]){
    if( strcmp(str, "NAME")==0 ){
        return NAME_F;
    }
    else if( strcmp(str, "TYPE")==0 ){
        return TYPE_F;
    }
    else if( strcmp(str, "DIMENSION")==0 ){
        return DIMENSION_F;
    }
    else if( strcmp(str, "EDGE_WEIGHT_TYPE")==0 ){
        return EDGE_WEIGHT_TYPE_F;
    }
    else if( strcmp(str, "COMMENT")==0 ){
        return COMMENT_F;
    }
    else{
        return 0;
    }
}

/// Initializes an instance_t structure
/// Does not allocate anything, just sets all variables to 0
void initTSPinstance(instance_t *instance){
    memset(instance->name, 0, MAXNAMELENGTH);
    memset(instance->type, 0, MAXNAMELENGTH);
    memset(instance->edge_type, 0, MAXNAMELENGTH);
    instance->dimension = 0;
    instance->length = 0;
}

/// Transforms a string like this:
/// ->|   fs qf   0|<-
/// into this:
/// ->|fs qf0000000|<-
void trim(char str[]){
    int i = 0;
    while(str[i] == ' ' && str[i] != 0){
        i++;
    }
    int n = 0;
    while(str[n] != 0){
        n++;
    }
    int j = n;
    while( j >= 0 && (str[j] == 0 || str[j] == ' ') ){
        j--;
    }
    int foo = 0;
    while(i <= j){
        str[foo] = str[i];
        foo++; i++;
    }
    while(foo<n){
        str[foo] = 0;
        foo++;
    }
}

/// Prints the contents of a tsp instance in the file pointed to by fp
void printTSPinstance(FILE *fp, instance_t instance){
    fprintf(fp, "\n\n========== TSP INSTANCE %s ==========\n", instance.name);
    fprintf(fp, "Name :              %s\n", instance.name);
    fprintf(fp, "Type :              %s\n", instance.type);
    fprintf(fp, "Dimension :         %d\n", instance.dimension);
    fprintf(fp, "Edge type :         %s\n", instance.edge_type);
    fprintf(fp, "Length :            %lf\n", instance.length);
    fprintf(fp, "tabCoord : \n");
    for(int i=0; i<instance.dimension; i++){
        fprintf(fp, "%d      %d      %d\n", i, instance.tabCoord[i][0], instance.tabCoord[i][1]);
    }
    fprintf(fp, "\n\n========== TSP INSTANCE %s ========== END\n", instance.name);
}