#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stack.h"

#define MAX_PUSH 4

mystack_t *pilha;

/* Threads que fazem push na pilha */
void * thread_push(void *arg) {
  int valor;
  int thread_id = *((int *) arg);
  for(int i=0; i < MAX_PUSH; i++) {
    valor = rand()%100;
    if(stack_push(pilha, valor))
      printf("Thread Push %d inseriu %d\n", thread_id, valor);
    else
      printf("Thread Push %d nao pode inserir %d\n", thread_id, valor);
  }
}

/* Threads que fazem pop na pilha */
void * thread_pop(void *arg) {
  int valor;
  int thread_id = *((int *) arg);

  while(stack_pop(pilha, &valor) != 0)
    printf("Thread Pop %d removeu %d\n", thread_id, valor);
}

int main(int argc, char **argv) {

  if(argc != 2) {
    printf("Execute %s <numero threads>\n", argv[0]);
    return 1;
  }

  srand(time(NULL));
  int numero_threads = atoi(argv[1]);
  int thread_ids[numero_threads];
  pthread_t threads[numero_threads];

  pilha = stack_alloc(numero_threads * MAX_PUSH);

  /* Cria as threads que fazem push */
  for(int i=0; i<numero_threads; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, thread_push, (void *)&thread_ids[i]);
  }

  /* Aguarda o termino das threads que fazem push */
  for(int i=0; i<numero_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Cria as threads que fazem pop */
  for(int i=0; i<numero_threads; i++)
    pthread_create(&threads[i], NULL, thread_pop, (void *)&thread_ids[i]);

  /* Aguarda o termino das threads que fazem pop */
  for(int i=0; i<numero_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
