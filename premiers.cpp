#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include "Chrono.hpp"

unsigned long lMax = 1000;
char *lFlags;

// odd variables
unsigned long gOdd = 3;
pthread_mutex_t gMutexOdd = PTHREAD_MUTEX_INITIALIZER;

// even variables
unsigned long gEven = 4;
unsigned long evenSlice = 10000;
pthread_mutex_t gMutexEven = PTHREAD_MUTEX_INITIALIZER;


void* eratosthenes(void* iArg)
{
    // even numbers
    while(gEven <= lMax) {
        pthread_mutex_lock(&gMutexEven);
        unsigned long from = gEven;
        gEven+=evenSlice;
        pthread_mutex_unlock(&gMutexEven);
        unsigned long to = from + evenSlice;
        if(to > lMax){
            to = lMax;
        }
        for (unsigned long i=from; i < to; i+=2) {
            lFlags[i]++;
        }
    }

    // odd numbers
    while(gOdd*gOdd <= lMax) {
        pthread_mutex_lock(&gMutexOdd);
        unsigned long current = gOdd;
        gOdd+=2;
        pthread_mutex_unlock(&gMutexOdd);
        if (lFlags[current] == 0) {
            // invalider tous les multiples
            for (unsigned long i=current; i*current < lMax; i+=2) {
                lFlags[i*current]++;
            }
        }
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
 
    // Allouer le tableau des drapeaux (flags) d'invalidation
    lFlags = (char*) calloc(lMax, sizeof(*lFlags));
    assert(lFlags != 0);

    //unsigned long i = 3;
    //printf("%lu \n", i/2);

    // premiere passe sur tout les multiple de 2
    /*for(unsigned long p=4; p < lMax; p+=2){
        lFlags[p]++;
    }*/

    // Appliquer la passoire d'Ératosthène
    pthread_t lIds[lThreads];
    for(int i=1; i<=lThreads; ++i) {
        pthread_create(&lIds[i-1], NULL, eratosthenes, NULL);
    }

    // wait for thread completion
    for(int i=1; i<=lThreads; ++i) {
        pthread_join(lIds[i-1], NULL);
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