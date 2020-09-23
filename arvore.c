#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>

void Pesquisa(TipoRegistro *x, TipoApontador *p) {
    if (*p == NULL) {
        printf("Erro: Registro %ld nao esta presente na arvore\n", x->Chave);
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

void* InsereParalelo(void *data){
    TArgs *args = (TArgs*) data;    
    Insere(args->x, args->p);
    return NULL;
}

void Insere(TipoRegistro x, TipoApontador *p) {
    if (*p == NULL) {
        *p = (TipoApontador)malloc(sizeof(TipoNo));

        /*
            Nao sei se faria sentido bloquear aqui 
            ou teriamos uma variavel de condicao pra 
            ver se o valor ta bloqueado pelo mutex ou nao
        */
        //pthread_mutex_lock (&x.Mutex);
        (*p)->Reg = x;
        (*p)->Esq = NULL;
        (*p)->Dir = NULL;
        //pthread_mutex_unlock (&x.Mutex);

        return;
    }

    if (x.Chave < (*p)->Reg.Chave) {
        Insere(x, &(*p)->Esq);
        return;
    }

    if (x.Chave > (*p)->Reg.Chave) {
        Insere(x, &(*p)->Dir);
    } else {
        printf("Erro: Registro ja existe na arvore\n");
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
        printf("Erro : Registro %ld nao esta na arvore\n", x.Chave);
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
        pthread_mutex_lock (&x.Mutex);
        Aux = *p;
        *p = (*p)->Esq;
        free(Aux);
        pthread_mutex_unlock (&x.Mutex);
        return;
    }

    if ((*p)->Esq != NULL) {
        Antecessor(*p, &(*p)->Esq);
        return;
    }
    
    pthread_mutex_lock (&x.Mutex);
    Aux = *p;
    *p = (*p)->Dir;
    free(Aux);
    pthread_mutex_unlock (&x.Mutex);
}

void Central(TipoApontador p) {
    if (p == NULL)
        return;
    Central(p->Esq);
    printf("%ld\n", p->Reg.Chave);
    Central(p->Dir);
}