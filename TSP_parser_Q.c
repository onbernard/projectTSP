#include "TSP_parser_Q.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "basic_Q.h"
#include "error.h"

// TODO
/*
int parseTSPFile(FILE *fp, instance_t *instance){

}
*/

int main(int argc, char **argv){
    FILE *fp = fopen("test.txt", "r");
    if(fp == NULL){
        return 0;
    }
    tokenize(fp);
    while(!is_empty_Q()){
        char *buffer = pop_Q();
        printf("%s\n", buffer);
        free(buffer);
    }
    fclose(fp);
}

/*
int parseSpecs(instance_t *instance){
    char buffer[TOKENSIZE];
    while(pop_Q(buffer)){

    }
}
*/
// END TODO
int divide(char left[], char right[]){
    int i = 0;
    memset(left, 0, TOKENSIZE);
    memset(right, 0, TOKENSIZE);
    while(i<TOKENSIZE-1)
}

void tokenize(FILE *fp){
    int l = 0;
    char c;
    while( (c = fgetc(fp)) != EOF ){
        l++;
        if(c == '\n'){
            if(l>1){
                if( fseek(fp, -l, SEEK_CUR) != 0 ){
                    error();
                    return;
                }
                char *buffer = (char *) malloc(l * sizeof(char));
                if(buffer == NULL){
                    error();
                    return;
                }
                for(int i=0; i<l; i++){
                    buffer[i] = fgetc(fp);
                }
                buffer[l-1] = 0;
                l = 0;
                //trim(buffer);
                push_Q(buffer);
            }
            else{
                l = 0;
            }
        }
    }
    if(l == 0){
        return;
    }
    if( fseek(fp, -l, SEEK_CUR) != 0 ){
        error();
        return;
    }
    char *buffer = (char *) malloc((l+1) * sizeof(char));
    if(buffer == NULL){
        error();
        return;
    }
    for(int i=0; i<l; i++){
        buffer[i] = fgetc(fp);
    }
    buffer[l] = 0;
    //trim(buffer);
    push_Q(buffer);
}

/*
void trim(char buffer[]){
    int i = 0;
    while(buffer[i] == ' ' && buffer[i] != 0){
        i++;
    }
    int j = TOKENSIZE-2;
    while( j >= 0 && (buffer[j] == 0 || buffer[j] == ' ') ){
        j--;
    }
    int foo = 0;
    while(i <= j){
        buffer[foo] = buffer[i];
        foo++; i++;
    }
    while(foo<TOKENSIZE){
        buffer[foo] = 0;
        foo++;
    }
}*/

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