#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

int produzir(int *seed);
void consumir(int produto);

sem_t semaforoProd;
sem_t semaforoCons;

void *ProdutorFunc(void *arg);
void *ConsumidorFunc(void *arg);

int indice_produtor, indice_consumidor, tamanho_buffer, num_prod, num_cons;
int* buffer;

//Você deve fazer as alterações necessárias nesta função e na função
//ConsumidorFunc para que usem semáforos para coordenar a produção
//e consumo de elementos do buffer.

void *ProdutorFunc(void *arg) {
    int seed = time(NULL); //Seed para o gerador de números aleatórios
    int id = *((int*) arg);
    while (true) {
        printf("Produtor %d\n", id);        
        int produto = produzir(&seed); //produz um elemento usando o seed do parâmetro
        sem_wait(&semaforoProd);
        indice_produtor = (indice_produtor + 1) % tamanho_buffer; //calcula o próximo elemento
        buffer[indice_produtor] = produto; //adiciona o elemento produzido à lista
        sem_post(&semaforoCons);
    }
}

void *ConsumidorFunc(void *arg) {
    int id = *((int*) arg);
    while (true) {       
        printf("Consumidor %d\n", id);
        int produto;
         sem_wait(&semaforoCons);
        indice_consumidor = (indice_consumidor + 1) % tamanho_buffer; //Calcula o próximo item a consumir
        produto = buffer[indice_consumidor]; //obtém o item da lista
        consumir(produto); //Consome o item obtido.
        sem_post(&semaforoProd);
    }
}

int main(int argc, char *argv[]) {
    
    if (argc < 4) {
        printf("Use: %s [tamanho do buffer]\n", argv[0]);
        return 0;
    }

    tamanho_buffer = atoi(argv[1]);
    num_prod = atoi(argv[2]);
    num_cons = atoi(argv[3]);

    indice_produtor = 0;
    indice_consumidor = 0;
    sem_init(&semaforoCons, 0 , 0);
    sem_init(&semaforoProd, 0 , tamanho_buffer);

    //Iniciando buffer
    buffer = malloc(sizeof(int) * tamanho_buffer);

    //Aqui você deve criar as threads e semáforos necessários
    //para que o código funcione adequadamente.
    pthread_t threads_prod[num_prod];
    pthread_t threads_cons[num_cons];
    int prod_id[num_prod];
    int cons_id[num_cons];

    for(int i = 0; i < num_prod + num_cons; i++) {
		if(i < num_prod) {
			prod_id[i] = i;
			pthread_create(&threads_prod[i], NULL, ProdutorFunc, &prod_id[i]);
		}
		
		if(i < num_cons) {
			cons_id[i] = i;
			pthread_create(&threads_cons[i], NULL, ConsumidorFunc, &cons_id[i]);    
		}
    }
    
    for(int i = 0; i < num_prod + num_cons; i++) {
		if(i < num_prod)
			pthread_join(threads_prod[i], NULL);
		
		if(i < num_cons)
			pthread_join(threads_cons[i], NULL);
    }

    //Libera memória do buffer
    free(buffer);

    return 0;
}

//Esta função produz um elemento. O argumento seed é usado para gerar
//um número aleatório, e é necessário por que a função rand() não
//é thread-safe e tem comportamento indefinido se usada por várias
//threads ao mesmo tempo.
int produzir(int *seed) {
    int produto = 500 + (rand_r(seed) % 500);
    printf("Produzindo %d\n", produto);
    usleep(produto * 1000);
    return produto;
}

//Esta função consome um elemento.
void consumir(int produto) {
    printf("Consumindo %d\n", produto);
    usleep(produto * 1000);
}
