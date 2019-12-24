#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void swap(char str[], int i, int j){
    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;
    return;
}

int compare (const void *a, const void * b) 
{  return ( *(char *)a - *(char *)b ); } 

void sort(char str[], int start, int end){
    int pivot = start;
    int j = end-1;
    while(pivot<j){
        if( str[pivot+1] <= str[pivot] ){
            swap(str, pivot+1, pivot);
            pivot++;
        }
        else{
            swap(str, pivot+1, j);
            j--;
        }
    }
}

long int fact(int n){
    long int outp = 1;
    for(int i = 1; i<=n; i++){
        outp *= i;
        printf("%ld\n", outp);
    }
    return outp;
}


int main(int argc, char **argv){

    char str[9];
    strcpy(str, "ABCDEFGH");

    long int n = 1;
    printf("%s\n", str);

    while(1){
        int i = 0;
        int first = -1;
        while( i<7){
            if(str[i] < str[i+1]){
                first = i;
            }
            i++;
        }
        if(first == -1){
            break;
        }
        int j = first+1;
        int sec = first +1;
        while( j<8 ){
            if( str[j] > str[first] && str[j] < str[sec] ){
                sec = j;
            }
            j++;
        }
        //printf("first: %d  second: %d\n", first, sec);
        swap(str, first, sec);
        qsort( str + first + 1, 8 - first - 1, sizeof(str[0]), compare ); 
        printf("%s\n", str);
        n++;
    }

    printf("%ld   %ld\n\n", fact(8) , n);
}