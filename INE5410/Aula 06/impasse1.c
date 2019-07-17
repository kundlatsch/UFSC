#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void *worker1_func(void *arg);
void *worker2_func(void *arg);
int operacao_worker1(int *seed);
int operacao_worker2(int *seed);

int total_computado;
int *lista_de_operacoes;
int proximo_indice;

pthread_mutex_t mutex_total;
pthread_mutex_t mutex_lista;

void *worker1_func(void *arg) {
    int seed = time(NULL); //seed para produzir números aleatórios

    int contador = 0;
    while (contador < 20) {
        int operacao = operacao_worker1(&seed);

        pthread_mutex_lock(&mutex_total);
        printf("Worker 1 obteve mutex_total\n");
        total_computado += operacao;

        if (pthread_mutex_trylock(&mutex_lista) == 0) {
            printf("Worker 1 obteve mutex_lista\n");
            lista_de_operacoes[proximo_indice++] = operacao;
            pthread_mutex_unlock(&mutex_lista);
            
        }

        printf("Worker 1 liberou mutex_total\n");
        pthread_mutex_unlock(&mutex_total);
        
        printf("Worker 1 liberou mutex_lista\n");
        contador += 1;
    }
    printf("Worker 1 terminou\n");
}

void *worker2_func(void *arg) {
    int seed = time(NULL) * 2; //seed diferente para produzir números aleatórios

    int contador = 0;
    while (contador < 20) {
        pthread_mutex_lock(&mutex_lista);
        printf("Worker 2 obteve mutex_lista\n");
        lista_de_operacoes[proximo_indice] = operacao_worker2(&seed);  // chama a operação aqui
        
        if (pthread_mutex_trylock(&mutex_total) == 0) {
            printf("Worker 2 obteve mutex_total\n");
            total_computado += lista_de_operacoes[proximo_indice];
            proximo_indice += 1;
            pthread_mutex_unlock(&mutex_total);
        } 
        
        printf("Worker 2 liberou mutex_total\n");
        pthread_mutex_unlock(&mutex_lista);
        
        printf("Worker 2 liberou mutex_lista\n");
        contador += 1;
    }
    printf("Worker 2 terminou");
}

int operacao_worker1(int *seed) {
    //Produz um número aleatório
    int operacao = 500 + rand_r(seed)%500;
    printf("Worker 1 produziu %d\n", operacao);
    usleep(operacao * 1000);
    return operacao;
}

int operacao_worker2(int *seed) {
    //Produz um número aleatório
    int operacao = 300 + rand_r(seed)%300;
    printf("Worker 2 produziu %d\n", operacao);
    usleep(operacao * 1000);
    return operacao;
}

void main(int argc, char *argv[]) {
    //Inicia as variáveis globais
    proximo_indice = 0;
    lista_de_operacoes = malloc(sizeof(int) * 40);
    total_computado = 0;

    //Inicia os mutexes
    pthread_mutex_init(&mutex_lista, NULL);
    pthread_mutex_init(&mutex_total, NULL);

    //Cria as threads do worker1 e worker2.
    pthread_t worker1, worker2;
    pthread_create(&worker1, NULL, worker1_func, NULL);
    pthread_create(&worker2, NULL, worker2_func, NULL);

    //Join nas threads
    pthread_join(worker1, NULL);
    pthread_join(worker2, NULL);

    printf("Total das operações: %d\n", total_computado);

    //Libera mutexes e memória alocada
    pthread_mutex_destroy(&mutex_lista);
    pthread_mutex_destroy(&mutex_total);
    free(lista_de_operacoes);
}