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

#define NUM_THREAD 4

void TestaI(TipoNo *p, int pai);
void Testa(TipoNo *p);
double rand0a1();
void Permut(TipoChave A[], int n);

int main(int argc, char *argv[]) {
    struct timeval t;
    TipoNo *Dicionario;
    TipoRegistro x;
    TipoChave vetor[MAX_NODES];
    int i, j, k, n, rc;

    /* Iniciando barreira de sincronismo para testes da arvore */
    TBarreira bar, barExclusao, barPesquisa;
    initBarreira(&bar, MAX_NODES + 1);
    initBarreira(&barExclusao, 1);
    initBarreira(&barPesquisa, 1);

    Inicializa(&Dicionario);
    /* Gera uma permutação aleatoria de chaves entre 1 e MAX_NODES */
    for (i = 0; i < MAX_NODES; i++)
        vetor[i] = i + 1;

    gettimeofday(&t, NULL);
    srand((unsigned int)t.tv_usec);
    Permut(vetor, MAX_NODES - 1);

    pthread_t thread_id[MAX_NODES]; 
    TArgs args[MAX_NODES];
        
    /* Insere cada chave na arvore e testa sua integridade apos cada insercao */
    for (i = 0; i < MAX_NODES; i++) {
        //Inicia conteudo do No da arvore que sera inserido
        x.Chave = vetor[i];
        pthread_mutex_init (&x.Mutex, NULL);
        pthread_cond_init(&x.Condicao, NULL);
        
        //Adiciona lista de parametros para execucao da Insercao em paralelo
        args[i].x = x;
        args[i].p = (TipoApontador*) malloc(sizeof(TipoApontador));
        args[i].barreira = (TBarreira*) malloc(sizeof(TBarreira));
        args[i].p = &Dicionario;
        args[i].barreira = &bar;

        rc = pthread_create(&(thread_id[i]), NULL, InsereParalelo, &args[i]); 
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        Testa(Dicionario);
    }

    barreira(&bar);

    pthread_t thread2_id[MAX_NODES+1]; 
    TArgs argsRemove[MAX_NODES];
    pthread_t threadPesquisa_id[MAX_NODES][MAX_NODES+1]; 
    TArgs argsPesquisa[MAX_NODES][MAX_NODES];

    /* Retira uma chave aleatoriamente e realiza varias pesquisas */
    for (i = 0; i < MAX_NODES; i++) {
        k = (int)(10.0 * rand() / (RAND_MAX + 1.0));
        n = vetor[k];
        x.Chave = n;
        pthread_mutex_init (&x.Mutex, NULL);
        
        //Adiciona lista de parametros para execucao da Exclusao em paralelo
        argsRemove[i].x = x;
        argsRemove[i].p = (TipoApontador*) malloc(sizeof(TipoApontador));
        argsRemove[i].barreira = (TBarreira*) malloc(sizeof(TBarreira));
        argsRemove[i].p = &Dicionario;
        argsRemove[i].barreira = &barExclusao;

        rc = pthread_create(&(thread2_id[i]), NULL, RetiraParalelo, &argsRemove[i]); 
        barreira(&barExclusao);
        if (rc){
             printf("ERROR; return code from pthread_create() is %d\n", rc);
             exit(-1);
         }

        Testa(Dicionario);

        printf("Retirou chave: %ld\n", x.Chave);
        for (j = 0; j < MAX_NODES; j++) {
            //Adiciona lista de parametros para execucao da Pesquisa em paralelo
            argsPesquisa[i][j].x = x;
            argsPesquisa[i][j].p = (TipoApontador*) malloc(sizeof(TipoApontador));
            argsPesquisa[i][j].barreira = (TBarreira*) malloc(sizeof(TBarreira));
            argsPesquisa[i][j].p = &Dicionario;
            argsPesquisa[i][j].barreira = &barPesquisa;

            x.Chave = vetor[(int)(10.0 * rand() / (RAND_MAX + 1.0))];
            
            if (x.Chave != n) {
                printf("Pesquisando chave: %ld\n", x.Chave);
                rc = pthread_create(&(threadPesquisa_id[i][j]), NULL, PesquisaParalelo, &argsPesquisa[i][j]); 
                barreira(&barPesquisa);
                if (rc){
                    printf("ERROR; return code from pthread_create() is %d\n", rc);
                    exit(-1);
                }
            }
        }
        x.Chave = n;
        Insere(x, &Dicionario, &barExclusao);
        printf("Inseriu chave: %ld\n", x.Chave);
        Testa(Dicionario);
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