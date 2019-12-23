#include "TSP_parser_Q.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "basic_Q.h"

int main(int argc, char **argv){
    FILE *fp = fopen("test.TSP", "r");
    if(fp == NULL){
        fprintf(stderr, "fopen fail\n");
        return -1;
    }

    instance_t instance;
    memset(instance.name, 0, MAXNAMELENGTH);
    memset(instance.type, 0, MAXNAMELENGTH);
    memset(instance.edge_type, 0, MAXNAMELENGTH);
    instance.dimension = 0;
    instance.length = 0;

    int err = parseFile(fp, &instance);
    if(err < 0){
        fprintf(stderr, "ERROR in main\n");
        return -1;
    }

    printf("\n\n-----INSTANCE-----\n");
    printf("Name: %s\n", instance.name);
    printf("Type: %s\n", instance.type);
    printf("Dimension: %d\n", instance.dimension);
    printf("Edge type: %s\n", instance.edge_type);
    printf("Length: %lf\n", instance.length);
    printf("tabCoord: \n");
    for(int i=0; i<instance.dimension; i++){
        printf("%d %d %d\n", i+1, instance.tabCoord[i][0], instance.tabCoord[i][1]);
    }
    printf("DONE\n");

    return 0;
}


int parseFile(FILE *fp, instance_t *instance){
    int err;

    err = tokenize(fp);
    if(err < 0){
        fprintf(stderr, "ERROR : in parseFile : error in tokenize\n");
        return -1;
    }

    err = parseSpecs(instance);
    if(err < 0){
        fprintf(stderr, "ERROR : in parseFile : error in parseSpecs\n");
        return -1;
    }

    instance->length = instance->dimension;
    instance->tabCoord = (int **) malloc( instance->dimension * sizeof(int *) );
    if(instance->tabCoord == NULL){
        fprintf(stderr, "ERROR : in parseFile : error while allocating tabCoord\n");
        return -1;
    }
    for(int i=0; i<instance->dimension; i++){
        instance->tabCoord[i] = (int *) malloc( 2 * sizeof(int) );
        if(instance->tabCoord[i] == NULL){
            fprintf(stderr, "ERROR : in parseFile : error while allocating tabCoord[%d]\n", i);
            return -1;
        }
    }

    err = parseData(instance);
    if(err < 0){
        fprintf(stderr, "ERROR : in parseFile : error in parseData\n");
        return -1;
    }

    char *buffer = pop_Q();
    if(buffer == NULL){
        fprintf(stderr, "ERROR : in parseFile : expected EOF, ran out of token\n");
        return -1;
    }
    trim(buffer);
    if(strcmp(buffer, "EOF") != 0){
        fprintf(stderr, "ERROR : in parseFile : expected EOF, got #%s#\n", buffer);
        return -1;
    }
    free(buffer);

    buffer = pop_Q();
    if(buffer != NULL){
        fprintf(stderr, "ERROR : in parseFile : tokens after EOF\n");
        return -1;
    }

    return 0;
}


int parseData(instance_t *instance){
    char *buffer;

    buffer = pop_Q(); // POP

    if(buffer == NULL){
        fprintf(stderr, "ERROR : in parseData : reached end of queue\n");
        return -1;
    }
    if(specKeyword(buffer) != NODE_COORD_SECTION_F){
        fprintf(stderr, "ERROR : in parseData : expected NODE_COORD_SECTION\n");
        return -1;
    }

    free(buffer); // FREE

    int i = 0;
    int dim = instance->dimension;

    int nLine;
    double x;
    double y;

    while( i<dim ){
        buffer = pop_Q(); // POP

        if(buffer == NULL){
            fprintf(stderr, "ERROR : in parseData : reached end of queue\n");
            return -1;
        }

        int err = parseDataLine(buffer, &nLine, &x, &y); // PARSE

        if(err < 0){
            fprintf(stderr, "ERROR : in parseData : error in parseDataLine\n");
            return -1;
        }
        if(nLine != i+1){
            fprintf(stderr, "ERROR : in parseData : wrong line number\n");
            return -1;
        }

        instance->tabCoord[i][0] = x; // SET
        instance->tabCoord[i][1] = y; // SET

        free(buffer); // FREE

        i++;
    }
    return 0;
}

int parseDataLine(char line[], int *nLineBuff, double *xBuff, double *yBuff){
    if(line == NULL){
        fprintf(stderr, "ERROR : in parseDataLine : null buffer\n");
        return -2;
    }
    printf("%s\n", line);
    int firstSep = spaceDivide(line);
    printf("%d\n", firstSep);
    if(firstSep < 2){
        fprintf(stderr, "ERROR : in parseDataLine : length of first argument not correct\n");
        return -2;
    }
    int n = atoi(line);
    if(n > 0){
         *nLineBuff = n;
    }
    else{
        fprintf(stderr, "ERROR : in parseDataLine : could not convert first word to positive integer\n");
        return -2;
    }

    int secondSep = spaceDivide(line+firstSep);
    if(secondSep < 2){
        fprintf(stderr, "ERROR : in parseDataLine : length of second argument not correct\n");
        return -1;
    }
    double x = atof(line+firstSep);
    if(x > 0){
        *xBuff = x;
    }
    else{
        fprintf(stderr, "ERROR : in parseDataLine : could not convert second argument to double");
        return -1;
    }
    trim(line+firstSep+secondSep);
    if(strlen(line+firstSep+secondSep) < 1 ){
        fprintf(stderr, "ERROR : in parseDataLine : length of third argument not correct");
        return -1;
    }
    double y = atof(line+firstSep+secondSep);
    if(y > 0){
        *yBuff = y;
    }
    else{
        fprintf(stderr, "ERROR : in parseDataLine : could not convert third argument to double");
        return -1;
    }
    return 0;
}

int spaceDivide(char str[]){
    trim(str);
    int i = 0;
    while(str[i] != ' ' && str[i] != 0 ){
        i++;
    }
    if(str[i] == 0){
        return -1;
    }
    str[i] = 0;
    return i+1;
}

int parseSpecs(instance_t *instance){
    char *buffer;
    int specs = 0b0;
    while( specs != 0b1111 && ((buffer=pop_Q()) != NULL) ){ // #  ***  :  ******  0#
        printf("%s\n", buffer);

        int separator = colonDivide(buffer); // #  ***  0  ******  0#
        if(separator < 0){
            fprintf(stderr, "ERROR : in parseSpecs : expected ':' but did not find any\n");
            return -1;
        }
        trim(buffer); // #***0  ******  0#
        int keyword = specKeyword(buffer);

        printf("%s\n", buffer);
        printf("%s\n", buffer+separator);
        if(keyword < 0){
            fprintf(stderr, "ERROR : in parseSpecs : %s does not match any keyword\n", buffer);
            return -1;
        }
        if((specs&keyword) != 0){
            fprintf(stderr, "ERROR : in parseSpecs : keyword already specified\n");
            return -1;
        }
        int err;
        switch(keyword){
            case NAME_F: ;
                err = readName(instance->name, buffer+separator);
                if(err){
                    fprintf(stderr, "ERROR : in parseSpecs : error in getName");
                    return -1;
                }
                specs += NAME_F;
                break;
            case TYPE_F: ;
                err = readType(instance->type, buffer+separator);
                if(err){
                    fprintf(stderr, "ERROR : in parseSpecs : error in getName");
                    return -1;
                }
                specs += TYPE_F;
                break;
            case DIMENSION_F: ;
                int dim = readDimension(buffer+separator);
                if(dim > 0){
                    instance->dimension = dim;
                    specs += DIMENSION_F;
                }
                else{
                    fprintf(stderr, "ERROR : in parseSpecs : error in getDimension\n");
                    return -1;
                }
                break;
            case EDGE_WEIGHT_TYPE_F: ;
                err = readEdgeType(instance->edge_type, buffer+separator);
                if(err){
                    fprintf(stderr, "ERROR : in parseSpecs : error in getEdgeType\n");
                    return -1;
                }
                specs += EDGE_WEIGHT_TYPE_F;
                break;
            case COMMENT_F:
                readComment(buffer+separator);
                break;
        }
        free(buffer);
    }
    if(buffer == NULL){
        fprintf(stderr, "ERROR : in parseSpecs : reached end of queue\n");
        return -1;
    }
    if(specs != 0b1111){
        fprintf(stderr, "ERROR : in parseSpecs : not all keywords are there\n");
        return -1;
    }
    return 0;
}

int readName(char dest[], char src[]){
    trim(src); // #***0******0#
    int len = strlen(src);
    if(len == 0 || len >= MAXNAMELENGTH){
        fprintf(stderr, "ERROR : in getName : length does not fit\n");
        return -1;
    }
    strcpy(dest, src);
    return 0;
}

int readType(char dest[], char src[]){
    trim(src);
    if(strcmp(src, "TSP")){
        fprintf(stderr, "ERROR : non TSP types not supported\n");
        return -1;
    }
    strcpy(dest, src);
    return 0;
}

int readDimension(char src[]){
    trim(src);
    int dim = atoi(src);
    if(dim>0){
        return dim;
    }
    else{
        fprintf(stderr, "ERROR : in getDimension : could not  convert to positive integer\n");
        return -1;
    }
}

int readEdgeType(char dest[], char src[]){
    trim(src);
    if(strcmp(src, "EUC_2D")){
        fprintf(stderr, "ERROR : in getEdgeType : non EUC_2D format not supported\n");
        return -1;
    }
    strcpy(dest, src);
    return 0;
}

int readComment(char comment[]){
    printf("%s\n", comment);
    return 0;
}

int colonDivide(char str[]){
    int i = 0;
    while(str[i] != ':' && str[i] != 0 ){
        i++;
    }
    if(str[i] == 0){
        return -1;
    }
    str[i] = 0;
    return i+1;
}

int tokenize(FILE *fp){
    int l = 0;
    char c;
    while( (c = fgetc(fp)) != EOF ){
        l++;
        if(c == '\n'){
            if(l>1){
                if( fseek(fp, -l, SEEK_CUR) != 0 ){
                    fprintf(stderr, "ERROR : in tokenize : error in fseek\n");
                    return -1;
                }
                char *buffer = (char *) malloc(l * sizeof(char));
                if(buffer == NULL){
                    fprintf(stderr, "ERROR : in tokenize : error in malloc\n");
                    return -1;
                }
                for(int i=0; i<l; i++){
                    buffer[i] = fgetc(fp);
                }
                buffer[l-1] = 0;
                l = 0;
                push_Q(buffer);
            }
            else{
                l = 0;
            }
        }
    }
    if(l == 0){
        return 0;
    }
    if( fseek(fp, -l, SEEK_CUR) != 0 ){
        fprintf(stderr, "ERROR : in tokenize : error in fseek\n");
        return -1;
    }
    char *buffer = (char *) malloc((l+1) * sizeof(char));
    if(buffer == NULL){
        fprintf(stderr, "ERROR : in tokenize : error in malloc\n");
        return -1;
    }
    for(int i=0; i<l; i++){
        buffer[i] = fgetc(fp);
    }
    buffer[l] = 0;
    push_Q(buffer);
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

int specKeyword(const char str[]){
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
    else if( strcmp(str, "NODE_COORD_SECTION")==0 ){
        return NODE_COORD_SECTION_F;
    }
    else{
        return -1;
    }
}