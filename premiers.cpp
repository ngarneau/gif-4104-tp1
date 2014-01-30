#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "Chrono.hpp"

unsigned long lMax = 1000;
char *lFlags;

// Programme qui trouve à l'aide de la passoire d'Ératosthène,
// tous les nombres premiers inférieurs à un certain seuil 
// spécifié sur la ligne de commande.
// Attention, ce programme n'est aucunement optimisé!
int main(int argc, char *argv[])
{
    // Déterminer la limite supérieure pour la recherche;
    // par défaut, prendre 1000
    if (argc >= 2) {
        lMax = atol(argv[1]);
    }

    // Démarrer le chronomètre
    Chrono lChrono(true);
 
    // Allouer le tableau des drapeaux (flags) d'invalidation
    lFlags = (char*) calloc(lMax, sizeof(*lFlags));
    assert(lFlags != 0);

    //unsigned long i = 3;
    //printf("%lu \n", i/2);

    // premiere passe sur tout les multiple de 2
    for(unsigned long p=2; p < lMax; p+=2){
        lFlags[p]++;
    }

    // Appliquer la passoire d'Ératosthène
    for (unsigned long p=3; p*p < lMax; p+=2) {
        if (lFlags[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=p; i*p < lMax; i+=2) {
                lFlags[i*p]++;
            }
        }
    }

    // Arrêter le chronomètre
    lChrono.pause();

    // Afficher les nombres trouvés à la console
    for (unsigned long p=2; p<lMax; p++) {
        //if (lFlags[p] == 0) printf("%ld ", p);
    }
    printf("\n");

    // Afficher le temps d'exécution dans le stderr
    fprintf(stderr, "Temps d'execution = %f sec\n", lChrono.get());
 
    return 0;
}