#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "arg_parser.h"

_Bool verbose = 0;

FILE *logfile;

void parseArgumentsDynamicTest(){
    char input[128];
    args_t args;
    init_args_t(&args);
    int outp;
    _Bool stop = 0;
    char s[32];

    while(!stop){
        printf("ENTER A STRING (first word is  supposed to be tsp and is not checked) :\n");
        fgets(input, 128, stdin);
        input[strcspn(input, "\n")] = 0;
        char *argv[32];
        int argc = 0;
        argv[0] = strtok(input, " ");
    
        while( argv[argc] != NULL && argc < 32 ){
            argc++;
            argv[argc] = strtok(NULL, " ");
        }
        printf("\nVous avez entré:\n");
        for(int i=0; i<argc; i++){
            printf("%d : %s\n", i, argv[i]);
        }

        outp = parseArguments(argc, argv, &args);
        printf("\nreturn value: %d\n", outp);
        if(outp >= 0){
            printf("\nRésultat de parseArguments:\n");
            print_args(args);
            printf("\n");
        }

        printf("\nVOULEZ VOUS CONTINUER? Oui = o, non = other\n");
        fgets(s, 32, stdin);
        if(strcmp(s, "o\n") != 0){
            stop = 1;
        }
        else{
            stop = 0;
        }
    }
}

int main(int argc, char **argv){
    parseArgumentsDynamicTest();
    return 0;
}