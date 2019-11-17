#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "res.h"

/*
Usage :  ./tsp -f <file> [-t <tour>] [-v [<file>]] -<méthode> [-h]
-f <file> : nom du fichier tsp (obligatoire)
-t <file> : nom du fichier solution (optionnel)
-v [<file>] : mode verbose (optionnel), écrit dans un fichier si présent
-o <file> : export des résultats sans un fichier csv
-h : help, affiche ce texte

<méthode> : méthodes de calcul (cumulables) :

-bf : brute force,
-bfm : brute force en utilisant la matrice de distance,
-ppv : plus proche voisin,
-rw : random walk,
-2opt : 2 optimisation. Si -ppv ou -rw ne sont pas présentes on utilise -rw,
-ga <nombre d'individus> <nombre de générations> <taux de mutation> :
algorithme génétique, défaut = 20 individus, 200 générations, 0.3 mutation.
*/

char *TSPfileName = NULL;
char *TOURfileName = NULL;
_Bool verbose = 0;
char *logFileName = NULL;
FILE *logs = stdout
_Bool exportResults = 0;
char *exportFileName = NULL;
_Bool bf = 0;
_Bool bfm = 0;
_Bool ppv = 0;
_Bool rw = 0;
_Bool twoOpt = 0;
_Bool ga = 0;
int ga_nSpecimens = 20;
int ga_nGnerations = 200;
double ga_mutationRate = 0.3;

int parseArguments(int argc, char **argv){

}

int main(int argc, char **argv){
    

    if( parseArguments(int argc, char **argv) < 0 ){

    }
}