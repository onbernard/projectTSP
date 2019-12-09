#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>




int main(int argc, char **argv){

    FILE *fp = fopen("test.txt", "r");
    if(fp == NULL){
        return -1;
    }

    int i = 0;
    char c;
    while(i<4){
        c = fgetc(fp);
        printf("%c", c);
        i++;
    }
    if( fseek(fp, -4, SEEK_CUR) != 0 ){
        fclose(fp);
        return -1;
    }
    i = 0;
    while(i<4){
        c = fgetc(fp);
        printf("%c", c);
        i++;
    }
    fclose(fp);

    return 0;
}