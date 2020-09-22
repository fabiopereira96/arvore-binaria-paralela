#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>

void Pesquisa(TipoRegistro *x, TipoApontador *p) {
    if (*p == NULL) {
        printf("Erro: Registro nao esta presente na arvore\n");
        return;
    }
    if (x->Chave < (*p)->Reg.Chave) {
        Pesquisa(x, &(*p)->Esq);
        return;
    }

    if (x->Chave > (*p)->Reg.Chave) {
        Pesquisa(x, &(*p)->Dir);
    } else {
        *x = (*p)->Reg;
    }
}

void Insere(void *data) {
    TArgs *args = data;
    
    TipoApontador *p = args->p;
    TipoRegistro x = args->x;
    pthread_mutex_init (&(*p)->Mutex, NULL);
    pthread_mutex_lock (&(*p)->Mutex);
    if (*p == NULL) {
        *p = (TipoApontador)malloc(sizeof(TipoNo));
        (*p)->Reg = x;
        (*p)->Esq = NULL;
        (*p)->Dir = NULL;
        
        pthread_mutex_unlock (&(*p)->Mutex);
        return;
    }

    if (x.Chave < (*p)->Reg.Chave) {
        args->p = (*p)->Esq;
        Insere(args);
        pthread_mutex_unlock (&(*p)->Mutex);
        return;
    }

    if (x.Chave > (*p)->Reg.Chave) {
        args->p = (*p)->Dir;
        pthread_mutex_unlock (&(*p)->Mutex);
        Insere(args);

    } else {
        printf("Erro : Regist        ja existe na arvore\n");
    }
}

void Inicializa(TipoApontador *Dicionario) {
    *Dicionario = NULL;
}

void Antecessor(TipoApontador q, TipoApontador *r) {
    if ((*r)->Dir != NULL) {
        Antecessor(q, &(*r)->Dir);
        return;
    }
    q->Reg = (*r)->Reg;
    q = *r;
    *r = (*r)->Esq;
    free(q);
}

void Retira(TipoRegistro x, TipoApontador *p) {
    TipoApontador Aux;
    if (*p == NULL) {
        printf("Erro : Registro nao esta na arvore\n");
        return;
    }

    if (x.Chave < (*p)->Reg.Chave) {
        Retira(x, &(*p)->Esq);
        return;
    }

    if (x.Chave > (*p)->Reg.Chave) {
        Retira(x, &(*p)->Dir);
        return;
    }
    
    if ((*p)->Dir == NULL) {
        Aux = *p;
        *p = (*p)->Esq;
        free(Aux);
        return;
    }

    if ((*p)->Esq != NULL) {
        Antecessor(*p, &(*p)->Esq);
        return;
    }

    Aux = *p;
    *p = (*p)->Dir;
    free(Aux);
}

void Central(TipoApontador p) {
    if (p == NULL)
        return;
    Central(p->Esq);
    printf("%ld\n", p->Reg.Chave);
    Central(p->Dir);
}