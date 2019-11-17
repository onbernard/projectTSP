#ifndef _RES_H
#define _RES_H

char HELP[] = "Usage :  ./tsp -f <file> [-t <tour>] [-v [<file>]] -<méthode> [-h]\n-f <file> : nom du fichier tsp (obligatoire)\n-t <file> : nom du fichier solution (optionnel)\n-v [<file>] : mode verbose (optionnel), écrit dans un fichier si présent\n-o <file> : export des résultats sans un fichier csv\n-h : help, affiche ce texte\n\n<méthode> : méthodes de calcul (cumulables) :\n\n-bf : brute force,\n-bfm : brute force en utilisant la matrice de distance,\n-ppv : plus proche voisin,\n-rw : random walk,\n-2opt : 2 optimisation. Si -ppv ou -rw ne sont pas présentes on utilise -rw,\n-ga <nombre d'individus> <nombre de générations> <taux de mutation> :\nalgorithme génétique, défaut = 20 individus, 200 générations, 0.3 mutation.\n\n";


#endif