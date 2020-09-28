#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include <pthread.h>
#include "barreira.h"

#define MAX_NODES  10

typedef long TipoChave;

typedef pthread_mutex_t TipoMutex;
typedef pthread_cond_t TipoCondicao;

typedef struct TipoRegistro {
  TipoChave Chave;
  TipoMutex Mutex;
  TipoCondicao Condicao;
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
  TBarreira *barreira;
} TArgs;

void Inicializa(TipoApontador *Dicionario);

void Pesquisa(TipoRegistro *x, TipoApontador *p, TBarreira *bar);
void Insere(TipoRegistro x, TipoApontador *p, TBarreira *bar);
void Retira(TipoRegistro x, TipoApontador *p);

void Central(TipoApontador p);
void Antecessor(TipoApontador q, TipoApontador *r);

void* PesquisaParalelo (void *data);
void* InsereParalelo   (void *data);
void* RetiraParalelo   (void *data);

#endif