#ifndef PARALELISMO_H
#define PARALELISMO_H

#include <pthread.h>
#include <semaphore.h>

typedef struct {
    pthread_mutex_t mutex;
    sem_t waitsem;
    int nthreads, current;
} *TBarreira;

extern pthread_mutex_t mutex;

TBarreira bar;
void initBarreira(TBarreira* b, int n);
void barreira(TBarreira &bar);

#endif