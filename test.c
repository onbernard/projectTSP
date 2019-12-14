#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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


int main(int argc, char **argv){
    char str[] = ":";
    printf("#%s#\n", str);
    int sep = colonDivide(str);
    if(sep < 0){
        printf("STOP\n");
        return 1;
    }
    printf("#%s#\n", str);
    trim(str);
    printf("#%s#\n", str);
    printf("#%s#\n", str + sep);
    trim(str+sep);
    printf("#%s#\n", str + sep);
    return 0;
}