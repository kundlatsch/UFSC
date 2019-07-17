#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CAIXAS 3
#define NUM_CONTAS 5
#define TOTAL_TRANSFERENCIAS 100

typedef struct Conta_ {
  int id;
  double saldo;
  pthread_mutex_t mutex;
} Conta;

Conta contas[NUM_CONTAS];

void transferir(Conta *origem, Conta *destino, double valor);
void* caixa_func(void *arg);

//Voce deve alterar essa funcao de maneira que impasses sejam evitados,
//mas mantendo a funcionalidade dela (ou seja, retirar dinheiro de uma
//conta e colocar em outra). Voce tambem deve manter a chamada ao usleep()
//dentro da regiao critica.
//Nao altere nenhuma outra parte do codigo!
void transferir(Conta *origem, Conta *destino, double valor) {

  if(&origem->id < &destino->id){
    pthread_mutex_lock(&origem->mutex);
    pthread_mutex_lock(&destino->mutex);
  }
  else {
    pthread_mutex_lock(&destino->mutex);
    pthread_mutex_lock(&origem->mutex);
  }

  printf("Transferindo %.2f da conta %d para %d\n", valor, origem->id, destino->id);
  fflush(stdout);

  origem->saldo -= valor;
  destino->saldo += valor;
  usleep(500);

  pthread_mutex_unlock(&origem->mutex);
  pthread_mutex_unlock(&destino->mutex);
}


void *caixa_func(void *arg) {
  unsigned int seed = time(NULL);
  int conta1, conta2;
  
  for(int i = 0; i < TOTAL_TRANSFERENCIAS; i++) {
    //Escolhe duas contas diferentes aleatoriamente
    conta1 = rand_r(&seed) % NUM_CONTAS;
    conta2 = rand_r(&seed) % NUM_CONTAS;
    while(conta1 == conta2) conta2 = rand_r(&seed) % NUM_CONTAS;
    //Realiza a transferencia
    transferir(&contas[conta1], &contas[conta2], (double) 100.0 + (rand_r(&seed) & 900));
  }
}

int main(int argc, char* argv[]) {
  int i;
  
  //Cria as contas com algum saldo e inicializa o mutex de cada conta
  for (i = 0; i < NUM_CONTAS; i++) {
    contas[i].id = i;
    contas[i].saldo = 1000.00;
    pthread_mutex_init(&contas[i].mutex, NULL);
  }
  
  //Cria as threads
  pthread_t caixas[NUM_CAIXAS];
  for (i = 0; i < NUM_CAIXAS; i++)
    pthread_create(&caixas[i], NULL, caixa_func, NULL);
  
  //Esperar as threads terminarem
  for (i = 0; i < NUM_CAIXAS; i++)
    pthread_join(caixas[i], NULL);
  
  //Destroi os mutexes
  for (i = 0; i < NUM_CONTAS; i++)
    pthread_mutex_destroy(&contas[i].mutex);
  
  return 0;
}
