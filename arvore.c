#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>

void* PesquisaParalelo(void *data){
    TArgs *args = (TArgs*) data;    
    Pesquisa(&args->x, args->p, args->barreira);
    return NULL;
}

void Pesquisa(TipoRegistro *x, TipoApontador *p, TBarreira *bar) {
    if (*p == NULL) {
        printf("Erro: Registro %ld nao esta presente na arvore\n", x->Chave);
        return;
    }
    if (x->Chave < (*p)->Reg.Chave) {
        Pesquisa(x, &(*p)->Esq, bar);
        return;
    }

    if (x->Chave > (*p)->Reg.Chave) {
        Pesquisa(x, &(*p)->Dir, bar);
    } else {
        /*
           Aguarda até que o registro esteja desbloqueado para consulta
           Assim que chegamos ao No que contem o registro, aguardamos 
           ate que ele esteja disponivel apos um sinal via broadcast.
        */
        pthread_cond_wait(&(*p)->Reg.Condicao, &(*p)->Reg.Mutex);
        *x = (*p)->Reg;
    }
}

void* InsereParalelo(void *data){
    TArgs *args = (TArgs*) data;    
    Insere(args->x, args->p, args->barreira);
    return NULL;
}

void Insere(TipoRegistro x, TipoApontador *p, TBarreira *bar) {
    if (*p == NULL) {

        *p = (TipoApontador)malloc(sizeof(TipoNo));
        (*p)->Reg = x;
        (*p)->Esq = NULL;
        (*p)->Dir = NULL;
        
        /*
            Altera valor da barreira para sincronizar as threads no teste
            Notifica via broadcast todos os possiveis ouvintes
            para o registro x. 
            Na consulta temos um wait para essa mesma condicao.
        */
        printf("InseriuParalelo chave: %ld\n", x.Chave);
        barreira(bar);
        printf("Notifica todos ouvintes que aguardam a chave: %ld\n", x.Chave);
        pthread_cond_broadcast(&x.Condicao);

        return;
    }

    if (x.Chave < (*p)->Reg.Chave) {
        Insere(x, &(*p)->Esq, bar);
        return;
    }

    if (x.Chave > (*p)->Reg.Chave) {
        Insere(x, &(*p)->Dir, bar);
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

void* RetiraParalelo(void *data){
    TArgs *args = (TArgs*) data;    
    Retira(args->x, args->p);
    return NULL;
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
     /*
        Exclusao bloqueia o registro que será removido.
        Notifica via broadcast todos os possiveis ouvintes
        para o registro x apos liberar sua utilizacao. 
        Na consulta temos um wait para essa mesma condicao.
    */
    if ((*p)->Dir == NULL) {
        pthread_mutex_lock (&x.Mutex);
        Aux = *p;
        *p = (*p)->Esq;
        free(Aux);
        pthread_mutex_unlock (&x.Mutex);
        printf("Notifica todos ouvintes que aguardam a chave: %ld\n", x.Chave);
        pthread_cond_broadcast(&x.Condicao);
        return;
    }

    if ((*p)->Esq != NULL) {
        Antecessor(*p, &(*p)->Esq);
        return;
    }
    
    /*
        Exclusao bloqueia o registro que será removido.
        Notifica via broadcast todos os possiveis ouvintes
        para o registro x apos liberar sua utilizacao. 
        Na consulta temos um wait para essa mesma condicao.
    */
    pthread_mutex_lock (&x.Mutex);
    Aux = *p;
    *p = (*p)->Dir;
    free(Aux);
    pthread_mutex_unlock (&x.Mutex);
    printf("Notifica todos ouvintes que aguardam a chave: %ld\n", x.Chave);
    pthread_cond_broadcast(&x.Condicao);
}

void Central(TipoApontador p) {
    if (p == NULL)
        return;
    Central(p->Esq);
    printf("%ld\n", p->Reg.Chave);
    Central(p->Dir);
}