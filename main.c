/*
 * main.c
 *
 * Para executar, navegue até o caminho do arquivo e execute 
 * $ make run
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/time.h>
#include "arvore.h"
#include "barreira.h"

#define NUM_THREAD 1

void TestaI(TipoNo *p, int pai);
void Testa(TipoNo *p);
double rand0a1();
void Permut(TipoChave A[], int n);

int main(int argc, char *argv[]) {
    struct timeval t;
    TipoNo *Dicionario;
    TipoRegistro x;
    TipoChave vetor[MAX_NODES];
    int i, j, k, n;

    /* TESTANDO FUNCIONAMENTO DA BARREIRA IMPLEMENTADA */
    /*  - Numero de threads fixado em 1 */
    TBarreira bar;
    initBarreira(&bar, NUM_THREAD);
    barreira(&bar);
    destroyBarreira(&bar);

    Inicializa(&Dicionario);
    /* Gera uma permutação aleatoria de chaves entre 1 e MAX_NODES */
    for (i = 0; i < MAX_NODES; i++)
        vetor[i] = i + 1;

    gettimeofday(&t, NULL);
    srand((unsigned int)t.tv_usec);
    Permut(vetor, MAX_NODES - 1);

    /* Insere cada chave na arvore e testa sua integridade apos cada insercao */
    pthread_t thread_id[MAX_NODES]; 
        
    TArgs *args = malloc(sizeof(*args));
    int rc;
    for (i = 0; i < MAX_NODES; i++) {
        x.Chave = vetor[i];
        args->x = x;
        args->p = &Dicionario;

        rc = pthread_create(&thread_id[i], NULL, Insere, (void *)args); 
        if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
        printf("Inseriu chave: %ld\n", x.Chave);
        Testa(Dicionario);
    }

    /* Retira uma chave aleatoriamente e realiza varias pesquisas */
    for (i = 0; i <= MAX_NODES; i++) {
        k = (int)(10.0 * rand() / (RAND_MAX + 1.0));
        n = vetor[k];
        x.Chave = n;
        Retira(x, &Dicionario);
        Testa(Dicionario);
        printf("Retirou chave: %ld\n", x.Chave);
        for (j = 0; j < MAX_NODES; j++) {
            x.Chave = vetor[(int)(10.0 * rand() / (RAND_MAX + 1.0))];
            
            if (x.Chave != n) {
                printf("Pesquisando chave: %ld\n", x.Chave);
                Pesquisa(&x, &Dicionario);
            }
        }
        x.Chave = n;
        // Insere(x, &Dicionario);
        // printf("Inseriu chave: %ld\n", x.Chave);
        // Testa(Dicionario);
    }

    /* Retira a raiz da arvore ate que ela fique vazia */
    for (i = 0; i < MAX_NODES; i++) {
        x.Chave = Dicionario->Reg.Chave;
        Retira(x, &Dicionario);
        Testa(Dicionario);
        printf("Retirou chave: %ld\n", x.Chave);
    }

    return 0;
}


void TestaI(TipoNo *p, int pai) {
    if (p == NULL) return;

    if (p->Esq != NULL) {
        if (p->Reg.Chave < p->Esq->Reg.Chave) {
            printf("Erro: Pai %ld menor que filho a esquerda %ld\n", p->Reg.Chave,
                   p->Esq->Reg.Chave);
            exit(1);
        }
    }

    if (p->Dir != NULL) {
        if (p->Reg.Chave > p->Dir->Reg.Chave) {
            printf("Erro: Pai %ld maior que filho a direita %ld\n", p->Reg.Chave,
                   p->Dir->Reg.Chave);
            exit(1);
        }
    }

    TestaI(p->Esq, p->Reg.Chave);
    TestaI(p->Dir, p->Reg.Chave);
}

void Testa(TipoNo *p) {
    if (p != NULL) TestaI(p, p->Reg.Chave);
}

double rand0a1() {
    double resultado = (double)rand() / RAND_MAX; /* Dividir pelo maior inteiro */
    if (resultado > 1.0)
        resultado = 1.0;
    return resultado;
}

void Permut(TipoChave A[], int n) {
    int i, j;
    TipoChave b;
    for (i = n; i > 0; i--) {
        j = (i * rand0a1());
        b = A[i];
        A[i] = A[j];
        A[j] = b;
    }
}