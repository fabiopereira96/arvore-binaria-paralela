#include <pthread.h>
#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#define MAX_NODES  10

typedef long TipoChave;

typedef pthread_mutex_t TipoMutex;

typedef struct TipoRegistro {
  TipoChave Chave;
  TipoMutex Mutex;
} TipoRegistro;

typedef struct TipoNo * TipoApontador;

typedef struct TipoNo {
  TipoRegistro Reg;
  TipoApontador Esq, Dir;
} TipoNo;

typedef TipoApontador TipoDicionario;

typedef struct argStruct {
  TipoRegistro x;
  TipoApontador *p;
} TArgs;

void Pesquisa(TipoRegistro *x, TipoApontador *p);

void Inicializa(TipoApontador *Dicionario);
void Insere(TipoRegistro x, TipoApontador *p);
void Antecessor(TipoApontador q, TipoApontador *r);
void Retira(TipoRegistro x, TipoApontador *p);
void Central(TipoApontador p);

//pthreads
void* InsereParalelo(void *data);

#endif