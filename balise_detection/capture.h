#define _CAPTURE_H
#define _CAPTURE_H

// PRE  :   argc    :   numbers of arguments left in argv
//          argv    :   array of string with arguments to be read
//          buffer  :   buffer string where to write the filename
//
// POST :   return a negative number if not enough arguments are present in argv, 0 if they did not match, the numbers of matched arguments if they did
int capture_file(char **argv_left, int argc_left, char *buffer);

#endif