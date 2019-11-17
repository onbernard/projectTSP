#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg_parser.h"
#include "globals.h"

_Bool verbose = 0;
// points to log file. Default is stdout


int main(int argc, char **argv){
    args_t parsedArgs;
    init_args_t(&parsedArgs);

    printf("Return value: %d\n\n", parseArguments(argc, argv, &parsedArgs) );
    print_args_t(parsedArgs);
    return 0;
}