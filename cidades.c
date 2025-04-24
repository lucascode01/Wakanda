#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

int compararCidades(const void *a, const void *b) {
    return ((Cidade *)a)->Posicao - ((Cidade *)b)->Posicao;
}



Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) return NULL;

    int T, N;
    if (fscanf(arquivo, "%d", &T) != 1 || fscanf(arquivo, "%d", &N) != 1) {
        fclose(arquivo);
        return NULL;
    }

    if (T < 3 || T > 1000000 || N < 2 || N > 10000) {
        fclose(arquivo);
        return NULL;
    }

    Estrada *e = malloc(sizeof(Estrada));
    if (!e) {
        fclose(arquivo);
        return NULL;
    }


    e->T = T;
    e->N = N;
    e->C = malloc(sizeof(Cidade) * N);
    if (!e->C) {
        free(e);
        fclose(arquivo);
        return NULL;
    }
    
    for (int i = 0; i < N; i++) {
        int pos;
        char nome[256];

        if (fscanf(arquivo, "%d %[^\n]", &pos, nome) != 2 || pos <= 0 || pos >= T) {
            free(e->C);
            free(e);
            fclose(arquivo);
            return NULL;

        }

        for (int j = 0; j < i; j++) {

            if (e->C[j].Posicao == pos) {
                free(e->C);
                free(e);

                fclose(arquivo);
                return NULL;

            }
        }

        e->C[i].Posicao = pos;

        strcpy(e->C[i].Nome, nome);
    }





    fclose(arquivo);
    return e;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {

    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return -1;

    qsort(e->C, e->N, sizeof(Cidade), compararCidades);

    double menor = (e->C[1].Posicao - 0) / 2.0;

    for (int i = 1; i < e->N - 1; i++) {
        double vizinhanca = (e->C[i + 1].Posicao - e->C[i - 1].Posicao) / 2.0;
        if (vizinhanca < menor) menor = vizinhanca;
    }

    double ultima = (e->T - e->C[e->N - 2].Posicao) / 2.0;
    if (ultima < menor) menor = ultima;



    free(e->C);
    free(e);
    return menor;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return NULL;

    qsort(e->C, e->N, sizeof(Cidade), compararCidades);

    double menor = (e->C[1].Posicao - 0) / 2.0;
    char *nome = strdup(e->C[0].Nome);




    for (int i = 1; i < e->N - 1; i++) {



        double vizinhanca = (e->C[i + 1].Posicao - e->C[i - 1].Posicao) / 2.0;
        if (vizinhanca < menor) {
            menor = vizinhanca;
            free(nome);
            nome = strdup(e->C[i].Nome);
        }
    
    
    }

    double ultima = (e->T - e->C[e->N - 2].Posicao) / 2.0;



    if (ultima < menor) {
        free(nome);
        nome = strdup(e->C[e->N - 1].Nome);
    }

    free(e->C);

    free(e);

    return nome;
    
}

