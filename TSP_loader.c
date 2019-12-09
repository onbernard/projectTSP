#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TSP_loader.h"

#include "globals.h"
/*
_Bool isSpecKeyword(const char str[]){
    return !strcmp(str, "NAME") || !strcmp(str, "COMMENT") || !strcmp(str, "TYPE") || !strcmp(str, "DIMENSION") !strcmp(str, "EDGE_WEIGHT_TYPE");
}
*/

unsigned char readSpecKeyword(File *fp){
    char buffer[20];
    memset(buffer, 0, 20);
    int i=0;
    char c;
    while( i<19 && (c=fgetc(fp))!=EOF ){
        if( ( c < 'A' || c > 'Z' ) && c != '_' ){
            return 0;
        }
        if( c == '\n'){
            return 0;
        }
        buffer[i] = c;
        if(strcmp(buffer, "NAME")==0){
            return NAME_F;
        }
        else if(strcmp(buffer, "TYPE")==0){
            return TYPE_F;
        }
        else if(strcmp(buffer, "DIMENSION")==0){
            return DIMENSION_F;
        }
        else if(strcmp(buffer, "EDGE_WEIGHT_TYPE")==0){
            return EDGE_WEIGHT_TYPE_F;
        }
        else if(strcmp(buffer, "COMMENT")==0){
            return COMMENT_F;
        }
        else if(strcmp(buffer, "NODE_COORD_SECTION")==0){
            return NODE_COORD_SECTION_F;
        }
        ++i;
    }
    if( c == EOF ){
        fprintf(stderr, "ERROR : in readSpecKeyword : EOF reached\n");
        return EOF_REACHED_F;
    }
    fprintf(stderr, "ERROR : in readSpecKeyword : read 19 characters and didn'match anything\n");
    return 0;
}

int readSeperator(FILE *fp){
    int c;
    while( (c=fgetc(fp)) != EOF ){
        if(c == '\n'){
            fprintf(stderr, "ERROR : in readSeparator : end of line reached\n");
            return 0;
        }
        if(c == ':'){
            return 1;
        }
    }
    fprintf(stderr, "ERROR : in readSeparator : EOF reached\n");
    return 0;
}

int readName(FILE *fp, char buffer[]){
    memset(buffer, 0, MAXNAMELENGTH);
    char c;
    int i=0;
    while( i < MAXNAMELENGTH-1 && (c=fgetc(fp)) != EOF ){
        if( c == '\n'){
            if(i>0){
                return 1;
            }
            else{
                fprintf(stderr, "ERROR : in readName : \\n is encountered but no characters has been read\n");
                return 0;
            }
        }
        else if( (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ){
            buffer[i] = c;
            ++i;
        }
    }
    if( c == EOF ){
        fprintf(stderr, "ERROR : in readName : EOF reached\n");
    }
    if( i >= MAXNAMELENGTH-1 ){
        fprintf(stderr, "ERROR : in readName : string is too long to fit in buffer\n");
    }
    return 0;
}

int readType(FILE *fp){
    char buffer[5];
    memset(buffer, 0, 5);
    int i=0;
    char c;
    while( i<4 && (c=fgetc(fp))!=EOF ){
        if( c < 'A' || c > 'Z'){
            return 0;
        }
        buffer[i] = c;
        if(strcmp(buffer, "TSP")==0){
            return TSP_F;
        }
        else if(strcmp(buffer, "HCP")==0){
            return ATSP_F;
        }
        else if(strcmp(buffer, "ATSP")==0){
            return ATSP_F;
        }
        else if(strcmp(buffer, "SOP")==0){
            return SOP_F;
        }
        else if(strcmp(buffer, "CVRP")==0){
            return CVRP_F;
        }
        ++i;
    }
    if( c == EOF ){
        fprintf(stderr, "ERROR : in readType : EOF reached\n");
        return EOF_REACHED_F;
    }
    fprintf(stderr, "ERROR : in readType : read 4 characters and didn't match anything\n");
    return 0;
}

int readDimension(FILE *fp){
    int dimension = 0;
    char c;
    while( (c=fgetc(fp)) != EOF ){
        if( c > '0' && c < '9' ){
            dimension *= 10;
            dimension += c - '0';
        }
        if( c== '\n' ){
            return dimension;
        }
        else{
            fprintf(stderr, "ERROR : in readDimension : something other than a number or \\n has been read\n");
            return 0;
        }
    }
    fprintf(stderr, "ERROR : in readDimension : EOF reached\n");
    return 0;
}

int readEdgeWeightType(FILE *fp){
    char buffer[10];
    memset(buffer, 0, 10);
    int i=0;
    char c;
    while( i<9 && (c=fgetc(fp))!=EOF ){
        if( (c < 'A' || c > 'Z') && c != '_' && (c < '0' || c > '9') ){
            return 0;
        }
        buffer[i] = c;
        if(strcmp(buffer, "EXPLICIT")==0){
            return EXPLICIT_F;
        }
        else if(strcmp(buffer, "EUC_2D")==0){
            return EUC_2D_F;
        }
        else if(strcmp(buffer, "EUC_3D")==0){
            return EUC_3D_F;
        }
        else if(strcmp(buffer, "MAX_2D")==0){
            return MAX_2D_F;
        }
        else if(strcmp(buffer, "MAX_3D")==0){
            return MAX_3D_F;
        }
        else if(strcmp(buffer, "MAN_2D")==0){
            return MAN_2D_F;
        }
        else if(strcmp(buffer, "MAN_3D")==0){
            return MAN_3D_F;
        }
        else if(strcmp(buffer, "CEIL")==0){
            return CEIL_F;
        }
        else if(strcmp(buffer, "GEO")==0){
            return GEO_F;
        }
        else if(strcmp(buffer, "ATT")==0){
            return ATT_F;
        }
        else if(strcmp(buffer, "XRAY1")==0){
            return XRAY1_F;
        }
        else if(strcmp(buffer, "XRAY2")==0){
            return XRAY2_F;
        }
        else if(strcmp(buffer, "SPECIAL")==0){
            return SPECIAL_F;
        }
        ++i;
    }
    if( c == EOF ){
        fprintf(stderr, "ERROR : in readWeightType : EOF reached\n");
        return EOF_REACHED_F;
    }
    fprintf(stderr, "ERROR : in readWeightType : read 9 characters and didn't match anything\n");
    return 0;
}

int readComment(FILE *fp){
    char c;
    while( (c=fgetc(fp)) != EOF ){
        printf("%c", c);
        if(c == '\n'){
            printf("\n");
            return 1;
        }
    }
    fprintf(stderr, "ERROR : in readComment : EOF reached\n");
    return 0;
}

int readCoords(FILE *fp, )

// TODO : COMPLETE
int parseSpecs(FILE *fp, instance_t *TSP_instance){
    // ===================== SPECS READING ======================
    unsigned char specs = 0; // FLAGS
    unsigned char keyword;
    keyword = readSpecKeyword(fp);
    while(  keyword != NODE_COORD_SECTION_F ){
        if(keyword == 0){
            fprintf(stderr, "ERROR : in parseSpecs : readSpecKeyword error\n");
            return 0;
        }
        if(keyword == EOF_REACHED_F){
            fprintf(stderr, "ERROR : in parseSpecs : EOF is reached\n");
            return 0;
        }
        // ============================== NAME ===================================
        if(keyword == NAME_F){
            if( specs & NAME_F != 0 ){ // KEYWORD ALREADY SPECIFIED
                fprintf(stderr, "ERROR : in parseSpecs : name already specified\n");
                return 0;
            }
            if( !readSeperator(fp) ){ // READSEPARATOR HAS THROWN AN ERROR
                fprintf(stderr, "ERROR : in parseSpecs : readSeparator error\n");
                return 0;
            }
            if( !readName(fp, TSP_instance->name) ){ // READNAME HAS THROWN AN ERROR
                fprintf("ERROR : in parseSpecs : readName error\n");
                return 0;
            }
            printf("NAME CORRECTLY READ\n");
            specs += NAME_F;
        }
        // ============================== TYPE ===================================
        if(keyword == TYPE_F){
            if( specs & TYPE_F != 0 ){ // KEYWORD ALREADY SPECIFIED
                fprintf(stderr, "ERROR : in parseSpecs : type already specified\n");
                return 0;
            }
            if( !readSeperator(fp) ){ // READSEPARATOR HAS THROWN AN ERROR
                fprintf(stderr, "ERROR : in parseSpecs : readSeparator error\n");
                return 0;
            }
            if( readType(fp) != 1 ){
                fprintf(stderr, "ERROR : in parseSpecs : readType error\n");
                return 0;
            }
            strcpy(TSP_instance->type, "TSP");
            specs += TYPE_F;
        }
        // ============================== DIMENSION ===================================
        if(keyword == DIMENSION_F){
            if( specs & DIMENSION_F != 0 ){ // KEYWORD ALREADY SPECIFIED
                fprintf(stderr, "ERROR : in parseSpecs : dimension already specified\n");
                return 0;
            }
            if( !readSeperator(fp) ){ // READSEPARATOR HAS THROWN AN ERROR
                fprintf(stderr, "ERROR : in parseSpecs : readSeparator error\n");
                return 0;
            }
            if( (TSP_instance->dimension = readDimension(fp)) == 0 ){
                fprintf(stderr, "ERROR : in parseSpecs : readDimension error\n");
                return 0;
            }
            specs += DIMENSION_F;
        }
        // ============================== EDGE TYPE ===================================
        if(keyword == EDGE_WEIGHT_TYPE_F){
            if( specs & EDGE_WEIGHT_TYPE_F != 0 ){ // KEYWORD ALREADY SPECIFIED
                fprintf(stderr, "ERROR : in parseSpecs : edge_weight_type already specified\n");
                return 0;
            }
            if( !readSeperator(fp) ){ // READSEPARATOR HAS THROWN AN ERROR
                fprintf(stderr, "ERROR : in parseSpecs : readSeparator error\n");
                return 0;
            }
            if( readEdgeWeightType(fp) != 15 ){
                fprintf(stderr, "ERROR : in parseSpecs : readEdgeWeightType error\n");
                return 0;
            }
            strcpy(TSP_instance->edge_type, "ATT");
            specs += EDGE_WEIGHT_TYPE_F;
        }
        if(keyword == COMMENT_F){
            if( !readSeperator(fp) ){ // READSEPARATOR HAS THROWN AN ERROR
                fprintf(stderr, "ERROR : in parseSpecs : readSeparator error\n");
                return 0;
            }
            if( !readComment(fp) ){
                fprintf(stderr, "ERROR : in parseSpecs : readComment error\n");
                return 0;
            }
        }
        keyword = readSpecKeyword(fp);
    }
    if( specs != (NAME_F|TYPE_F|DIMENSION_F|EDGE_WEIGHT_TYPE_F) ){
        fprintf(stderr, "ERROR : in parseSpecs : not all flags have been specified\n");
        return 0;
    }
    return 1;
}