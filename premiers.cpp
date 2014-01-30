#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "Chrono.hpp"

unsigned long lMax = 1000;
char *lFlags;
const int sliceSize = 128*1024;

// process only odd numbers of a specified block
void eratosthenesBlockSimple(unsigned long from, unsigned long to)
{

  for (unsigned long i = 3; i*i <= to; i += 2)
  {
    // skip numbers before current slice
    unsigned long minJ = ((from+i-1)/i)*i;
    //printf("from: %lu\n", from);
    //printf("i: %lu\n", i);

    // start value must be odd
    if ((minJ & 1) == 0)
      minJ += i;

    //printf("minJ: %lu\n", minJ);

    // find all odd non-primes
    //unsigned long i=p; i*p < lMax; i+=2
    if (lFlags[minJ] == 0) {
        // invalider tous les multiples
        for (unsigned long i=minJ; i*minJ < to; i+=2) {
            lFlags[i*minJ]++;
        }
    }
    /*for (unsigned long j = minJ; j*minJ <= to; j += 2*i)
    {
      //unsigned long index = j - from;
      //lFlags[index/2]++;
        lFlags[j*minJ]++;
    }*/
  }
}

/**
 * On va créer ici plusieurs block
 * séparer la charge de travail
 */
void eratosthenesParBlock()
{
    int numBlocks = 0;
    // each slices covers ["from" ... "to"], incl. "from" and "to"
    for (unsigned long from = 2; from <= lMax; from += sliceSize)
    {
        unsigned long to = from + sliceSize;
        if (to > lMax) {
            to = lMax;
        }

        numBlocks++;

        eratosthenesBlockSimple(from, to);
    }

    //printf("Number of blocks: %d\n", numBlocks);
}

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

    eratosthenesParBlock();

    // Appliquer la passoire d'Ératosthène
    /*for (unsigned long p=3; p*p < lMax; p+=2) {
        if (lFlags[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=p; i*p < lMax; i+=2) {
                lFlags[i*p]++;
            }
        }
    }*/

    // Arrêter le chronomètre
    lChrono.pause();

    // Afficher les nombres trouvés à la console
    for (unsigned long p=2; p<lMax; p++) {
        if (lFlags[p] == 0) printf("%ld ", p);
    }
    printf("\n");

    // Afficher le temps d'exécution dans le stderr
    fprintf(stderr, "Temps d'execution = %f sec\n", lChrono.get());
 
    return 0;
}