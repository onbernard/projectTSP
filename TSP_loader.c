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
        ++i;
    }
    if( c == EOF ){
        fprintf(stderr, "ERROR : in readSpecKeyword : EOF reached\n");
        return EOF_REACHED_F;
    }
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
        if( c == '\n' ){
            return 1;
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
        fprintf(stderr, "ERROR : in readName : given name is too long\n");
    }
    return 0;
}
// TODO
//int readType

// TODO : COMPLETE
int parseTSP(FILE *fp, instance_t *TSP_instance , _Bool nz){
    // ===================== SPECS READING ======================
    unsigned char specs = 0; // FLAGS
    unsigned char keyword;
    while( ( keyword = readSpecKeyword(fp) ) != 0 ){
        if(keyword == NAME_F){
            if( specs & NAME_F != 0 ){ // KEYWORD ALREADY SPECIFIED
                fprintf(stderr, "ERROR : in parseTSP : name already specified\n");
                return 0;
            }
            if( !readSeperator(fp) ){ // READSEPARATOR HAS THROWN AN ERROR
                fprintf(stderr, "ERROR : in parseTSP : readSeparator error\n");
                return 0;
            }
            if( !readName(fp, TSP_instance->name) ){ // READNAME HAS THROWN AN ERROR
                fprintf("ERROR : in parseTSP : readName error\n");
                return 0;
            }
            if(  strlen(TSP_instance->name) == 0){ // NO NAME SPECIFIED
                fprintf(stderr, "ERROR : in parseTSP : name length is 0\n");
                return 0;
            }
            printf("NAME CORRECTLY READ\n");
            specs += NAME_F;
        }
        if(keyword == TYPE_F){
            if( specs & TYPE_F != 0 ){ // KEYWORD ALREADY SPECIFIED
                fprintf(stderr, "ERROR : in parseTSP : type already specified\n");
                return 0;
            }
            if( !readSeperator(fp) ){ // READSEPARATOR HAS THROWN AN ERROR
                fprintf(stderr, "ERROR : in parseTSP : readSeparator error\n");
                return 0;
            }
            specs += TYPE_F;
        }
    }
}