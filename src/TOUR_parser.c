#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TOUR_parser.h"

#include "TSP_parser.h"
#include "globals.h"

/// Parses a tour file and stores the results in the instance_t structure pointed by argument instance
/// If nz is 0 then the point (0,0) is automaticly added at the first index
/// It allocates instance->tabTour
/// CALLS ABORT ON ALLOCATION ERRORS
int parseTOURfile(FILE *fp, instance_t *instance, _Bool nz){
    char buffer[512]; // IF YOU CAN TWEET IN 512 CHARACTERS YOU CAN WRITE A LINE IN THAT SPACE
    char *err = buffer;
    unsigned char specs_FLAGS = 0;
    unsigned int nLine = 0;

    // =========================== SPECS PART =================================
    while( specs_FLAGS != (NAME_F|TYPE_F|DIMENSION_F) ){
        err = fgets(buffer, 512, fp);
        if(err == NULL){
            fprintf(stderr, "TOUR FILE FORMAT ERROR : eof reached at line %d\n", nLine);
            return -1;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        nLine++;

        char *firstHalf = strtok(buffer, ":");
        if(firstHalf == NULL){
            fprintf(stderr, "TOUR FILE FORMAT ERROR : empty line\n");
            return -1;
        }
        trim(firstHalf);

        char *secondHalf = strtok(NULL, ":");
        if(secondHalf == NULL){
            fprintf(stderr, "TOUR FILE FORMAT ERROR : no colon separator in line %d\n", nLine);
            return -1;
        }
        trim(secondHalf);

        char *rest = strtok(NULL, ":");
        if(rest != NULL){
            fprintf(stderr, "TSP FORMAT ERROR : two separators at line %d\n", nLine);
            return -1;
        }

        if(strlen(firstHalf) == 0){
            fprintf(stderr, "TOUR FILE FORMAT ERROR : empty first half of line %d\n", nLine);
            return -1;
        }
        if(strlen(secondHalf) == 0){
            fprintf(stderr,"TOUR FILE FORMAT ERROR : empty second half of line %d\n", nLine);
            return -1;
        }

        unsigned char keyword = specKeyword(firstHalf);
        if( (specs_FLAGS & keyword) != 0){
            fprintf(stderr, "TOUR FILE FORMAT ERROR : %s already specified\n", firstHalf);
            return -1;
        }
        switch(keyword){
            case NAME_F:
                strncpy(instance->name, secondHalf, MAXNAMELENGTH-1);
                specs_FLAGS += NAME_F;
                break;
            case TYPE_F:
                if( strcmp(secondHalf, "TOUR") !=0 ){
                    fprintf(stderr, "TOUR FILE FORMAT ERROR : type has to be TOUR\n");
                    return -1;
                }
                strcpy(instance->type, "TOUR");
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
            case COMMENT_F:
                if(verbose){
                    fprintf(logfileP, "Comment : %s\n", secondHalf);
                }
                break;
            default:
                fprintf(stderr, "TOUR FILE FORMAT ERROR : unexpected keyword : %s\n", firstHalf);
                return -1;
        } // switch
    } // specs keyword loop

    err = fgets(buffer, MAXNAMELENGTH, fp);
    if(err == NULL){
        fprintf(stderr, "TOUR FILE FORMAT ERROR : eof reached at line %d\n", nLine);
        return -1;
    }
    nLine++;

    buffer[strcspn(buffer, "\n")] = 0;
    trim(buffer);
    if( strcmp(buffer, "TOUR_SECTION") != 0 ){
        fprintf(stderr, "TOUR FILE FORMAT ERROR : unexpected keyword : %s\n", buffer);
        return -1;
    }

    instance->tabTour = (int *) malloc( instance->dimension * sizeof(int));
    if(instance->tabTour == NULL){
        fprintf(stderr, "ERROR : in parseTOURfile : error while allocating tabTour\nAborting...\n");
        abort();
    }

    int i;
    if(nz){
        i = 0;
    }
    else{
        instance->tabTour[0] = 0;
        i = 1;
    }
    while(i<instance->dimension){
        err = fgets(buffer, 512, fp);
        if(err == NULL){
            fprintf(stderr, "TOUR FILE FORMAT ERROR : eof reached at line %d\n", nLine);
            return -1;
        }
        nLine++;
        buffer[strcspn(buffer, "\n")] = 0;
        trim(buffer);
        if(strlen(buffer) == 0){
            fprintf(stderr, "TOUR FILE FORMAT ERROR : empty line at line %d\n", nLine);
            return -1;
        }
        int nCity = atoi(buffer);
        if(nCity <= 0){
            fprintf(stderr, "TOUR FILE FORMAT ERROR : city number not valid at line %d: %s\n", nLine, buffer);
            return -1;
        }
        if(nz){
            nCity--;
        }
        if(nCity >= instance->dimension){
            fprintf(stderr, "TOUR FILE FORMAT ERROR : city number not valid at line %d: %d\n", nLine, nCity);
            return -1;
        }
        if(isInArr(instance->tabTour, nCity, i)){
            fprintf(stderr, "TOUR FILE FORMAT ERROR : at line %d, city number %d already in tabTour\n", nLine, nCity);
            return -1;
        }

        instance->tabTour[i] = nCity;
        i++;
    } // while data part
    err = fgets(buffer, 512, fp);
    if(err == NULL){
        fprintf(stderr, "TOUR FILE FORMAT ERROR : eof reached at line %d\n", nLine);
        return -1;
    }
    nLine++;
    buffer[strcspn(buffer, "\n")] = 0;
    trim(buffer);
    if( strcmp(buffer, "-1") != 0 ){
        fprintf(stderr, "TOUR FILE FORMAT ERROR : expected -1 at line %d but got %s\n", nLine, buffer);
        return -1;
    }

    err = fgets(buffer, 512, fp);
    if(err == NULL){
        fprintf(stderr, "TOUR FILE FORMAT ERROR : eof reached at line %d\n", nLine);
        return -1;
    }
    nLine++;
    buffer[strcspn(buffer, "\n")] = 0;
    trim(buffer);
    if( strcmp(buffer, "EOF") != 0 ){
        fprintf(stderr, "TOUR FILE FORMAT ERROR : expected EOF at line %d but got %s\n", nLine, buffer);
        return -1;
    }
    err = fgets(buffer, 512, fp);
    if(err != NULL){
        fprintf(stderr, "TOUR FILE FORMAT ERROR : there are still lines after EOF\n");
        return -1;
    }
    return 0;
}

/// Checks if value is in array
/// used in parseTOURfile to check if every city number is unique
_Bool isInArr(int *arr, int value, unsigned int size){
    for(int i=0; i<size; i++){
        if(arr[i] == value){
            return 1;
        }
    }
    return 0;
}


/// Not quite a copy of initTSPinstance
void initTOURinstance(instance_t *instance){
    memset(instance->name, 0, MAXNAMELENGTH);
    memset(instance->type, 0, MAXNAMELENGTH);
    memset(instance->edge_type, 0, MAXNAMELENGTH);
    instance->dimension = 0;
    instance->length = 0;
    instance->tabTour = NULL;
}

/// Prints the contents of a tour instance in the file pointed to by fp
void printTOURinstance(FILE *fp, instance_t instance){
    fprintf(fp, "\n\n========== TOUR INSTANCE %s ==========\n", instance.name);
    fprintf(fp, "Name :              %s\n", instance.name);
    fprintf(fp, "Type :              %s\n", instance.type);
    fprintf(fp, "Dimension :         %d\n", instance.dimension);
    fprintf(fp, "Edge type :         %s\n", instance.edge_type);
    fprintf(fp, "Length :            %lf\n", instance.length);
    fprintf(fp, "tabTour : \n");
    for(int i=0; i<instance.dimension; i++){
        fprintf(fp, "%d      %d\n", i, instance.tabTour[i]);
    }
    fprintf(fp, "\n\n========== TOUR INSTANCE %s ========== END\n", instance.name);
}