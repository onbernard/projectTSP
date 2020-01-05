#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TSP_parser.h"

#include "globals.h"

int parseTSPfile(FILE *fp, instance_t *instance, _Bool nz){
    char buffer[512]; // IF YOU CAN TWEET IN 512 CHARACTERS YOU CAN WRITE A LINE IN THAT SPACE
    char *err = buffer;
    unsigned char specs_FLAGS = 0;
    unsigned int nLine = 0;

    while( specs_FLAGS != (NAME_F|TYPE_F|DIMENSION_F|EDGE_WEIGHT_TYPE_F) ){
        // ==================== GET A LINE ==========================
        err = fgets(buffer, 512, fp);
        if(err == NULL){
            fprintf(stderr, "TSP FILE FORMAT ERROR : eof reached at line %d\n", nLine);
            return -1;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        nLine++;
        //printf("%d %s\n", nLine, buffer);

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

    // DATA PART
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
        
        char *firstPart = strtok(buffer, " ");
        if(firstPart == NULL){
            fprintf(stderr, "TSP FILE FORMAT ERROR : empty line at line %d\n", nLine);
            return -1;
        }
        char *secondPart = strtok(NULL, " ");
        if(secondPart == NULL){
            fprintf(stderr, "TSP FILE FORMAT ERROR : invalid line format at line %d\n", nLine);
            return -1;
        }
        char *thirdPart = strtok(NULL, " ");
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

        nLine++;
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
    err = fgets(buffer, 512, fp);
    if(err != NULL){
        fprintf(stderr, "TSP FILE FORMAT ERROR : there are still lines after EOF\n");
        return -1;
    }
    return 0;
}

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

int initTSPinstance(instance_t *instance){
    memset(instance->name, 0, MAXNAMELENGTH);
    memset(instance->type, 0, MAXNAMELENGTH);
    memset(instance->edge_type, 0, MAXNAMELENGTH);
    instance->dimension = 0;
    instance->length = 0;
    return 0;
}

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