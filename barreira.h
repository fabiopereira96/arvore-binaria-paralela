/*
 * barrier.h
 *
 * This header file describes the "barrier" synchronization
 * construct. The type barrier_t describes the full state of the
 * barrier including the POSIX 1003.1c synchronization objects
 * necessary.
 *
 * A barrier causes threads to wait until a set of threads has
 * all "reached" the barrier. The number of threads required is
 * set when the barrier is initialized, and cannot be changed
 * except by reinitializing.
 * AUTHOR: http://ptgmedia.pearsoncmg.com/images/0201633922/sourcecode/barrier.c
 * LAST REVISED: 20/09/2020 by Fábio Pereira
 */
#include <pthread.h>

typedef struct barrier_tag {
    pthread_mutex_t     mutex;          /* Control access to barrier */
    pthread_cond_t      cv;             /* wait for barrier */
    int                 valid;          /* set when valid */
    int                 threshold;      /* number of threads required */
    int                 counter;        /* current number of threads */
    int                 cycle;          /* alternate wait cycles (0 or 1) */
} TBarreira;

#define BARRIER_VALID   0xdbcafe

/* Support static initialization of barriers */
#define BARRIER_INITIALIZER(cnt) \
    {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, \
    BARRIER_VALID, cnt, cnt, 0}

extern int initBarreira (TBarreira *barrier, int n);
extern int destroyBarreira (TBarreira *barrier);
extern int barreira (TBarreira *barrier); /* OBS: This name was fixed to maintain compatibility with test cases */