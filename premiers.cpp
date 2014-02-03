#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include "Chrono.hpp"

unsigned long lMax = 1000;
char *lFlags;
const int sliceSize = 128*1024;
int numBlocks = 0;

pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gMutexNumBlock = PTHREAD_MUTEX_INITIALIZER;

unsigned long gP = 2;

// process only odd numbers of a specified block
void eratosthenesBlockSimple(unsigned long from, unsigned long to)
{
    // prevent from removing 2
    if(from == 2){
        from = 4;
    }

    // invalid tout les nombre pair
    for(unsigned long p = from; p < to; p+=2){
        lFlags[p]++;
    }

    for (unsigned long i = 3; i*i <= to; i += 2)
    {

        unsigned long minJ = ((from+i-1)/i)*i;

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
    }
}

/**
 * On va créer ici plusieurs block
 * séparer la charge de travail
 */
void* eratosthenesParBlock(void* iArg)
{


    while(gP <= lMax) {
        //pthread_mutex_lock(&gMutexNumBlock);
        //numBlocks++;
        //pthread_mutex_unlock(&gMutexNumBlock);

        unsigned long to = gP + sliceSize;
        if (to > lMax) {
            to = lMax;
        }
        eratosthenesBlockSimple(gP, to);
        pthread_mutex_lock(&gMutex);
        gP+=sliceSize;
        pthread_mutex_unlock(&gMutex);
    }

    pthread_exit(NULL);
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

    // Déterminer le nombre de threads
    int lThreads = 1; // par défault = 1
    if (argc >= 3) {
        lThreads = atoi(argv[2]);
    }

    // Démarrer le chronomètre
    Chrono lChrono(true);

    printf("Nombre: %lu\n", lMax);
 
    // Allouer le tableau des drapeaux (flags) d'invalidation
    lFlags = (char*) calloc(lMax, sizeof(*lFlags));
    assert(lFlags != 0);

    // create threads
    pthread_t lIds[lThreads];
    for(int i=1; i<=lThreads; ++i) {
        pthread_create(&lIds[i-1], NULL, eratosthenesParBlock, NULL);
    }

    // wait for thread completion
    for(int i=1; i<=lThreads; ++i) {
        pthread_join(lIds[i-1], NULL);
    }

    // Arrêter le chronomètre
    lChrono.pause();


    printf("Number of blocks: %d\n", numBlocks);

    // Afficher les nombres trouvés à la console
    for (unsigned long p=2; p<lMax; p++) {
        //if (lFlags[p] == 0) printf("%ld ", p);
    }
    printf("\n");

    // Afficher le temps d'exécution dans le stderr
    fprintf(stderr, "Temps d'execution = %f sec\n", lChrono.get());
 
    return 0;
}