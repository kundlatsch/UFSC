#include <stdio.h>
#include <stdlib.h>

double *a;
double *b;

double prod;

void* thread_function(void* arg) {
    int i = *(int *) arg;
    prod += a[i] * b[i];
}

int main (int argc, char *argv[])
{
  int tam_vet;
  int i;
  prod = 0.0;


  tam_vet = atoi(argv[1]);
  pthread_t threads[tam_vet];

  if(argc<2){
    printf("uso %s <tamanho vetores>\n", argv[0]);
    exit(1);
  }

  /* alocacao do vetor A */
  a = (double *) malloc(sizeof(double) * tam_vet);

  /* alocacao do vetor B */
  b = (double *) malloc(sizeof(double) * tam_vet);

  printf("Inicializando vetores A e B...\n");

  /* inicializacao dos vetores */
  for (i=0; i<tam_vet; i++)
    a[i] = i;

  for (i=0; i<tam_vet; i++)
    b[i] = i;

  printf("Calculando...\n");

  /* produto escalar dos vetores */
  for (i=0; i < tam_vet; i++) {
      pthread_create(&threads[i], NULL, thread_function, &i);
  }

  for (i=0; i < tam_vet; i++) {
      pthread_join(threads[i], NULL);
  }


  printf("Terminou!\n");

  /*** imprime o resultado ***/
  printf("******************************************************\n");
  printf("Produto escalar: %.2f\n", prod);
  printf("******************************************************\n");

  free(a);
  free(b);
}