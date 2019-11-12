#include "capture.h"
#include "tools.h"
#include <stdlib.h>
#include <string.h>

// PRE  :   argc    :   numbers of arguments left in argv
//          argv    :   array of string with arguments to be read
//          buffer  :   buffer string where to write the filename
//
// POST :   return a negative number if not enough arguments are present in argv, 0 if they did not match, the numbers of matched arguments if they did
int capture_file(int argc_left, char **argv_left, char *buffer){
    if( argc <2 ){
        return argc - 2;
    }
    if( strcmp(argv[0],"-f") == 0 ){
        strcpy(buffer, argv_left[1], STRLENGTH);
        return 2;
    }
    else{
        return 0;
    }
}