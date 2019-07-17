#include <stdio.h>
#include <stdlib.h>

double retorno_thread = 0;

typedef struct arg_str{
  double a;
  double b;
};

void *func_thread(void *arg) {
  struct arg_str *args = (struct arg_str *)arg;
  printf("DEBUG FUNC: %f\n", &(*args).a);
  int retorno_thread = (*args).a + (*args).b;
  pthread_exit(&retorno_thread);
}

int main (int argc, char *argv[])
{
  int tam_vet;
  int i;
  int imprimir = 0;

  if(argc<2){
    printf("uso %s <tamanho vetores> [imprimir? 1=sim]\n", argv[0]);
    exit(1);
  }

  /* tamanho dos vetores */
  tam_vet = atoi(argv[1]);

  if(argc==3 && atoi(argv[2])==1)
    imprimir = 1;

  pthread_t threads[tam_vet]; 
  /* alocacao do vetor A */
  double *a = (double *) malloc(sizeof(double) * tam_vet);

  /* alocacao do vetor B */
  double *b = (double *) malloc(sizeof(double) * tam_vet);

  /* alocacao do vetor C */
  double *c = (double *) malloc(sizeof(double) * tam_vet);

  printf("Inicializando vetores A, B e C...\n");

  /* inicializacao dos vetores */
  for (i=0; i<tam_vet; i++)
    a[i] = i;

  for (i=0; i<tam_vet; i++)
    b[i] = i;

  for (i=0; i<tam_vet; i++)
    c[i] = 0;

  printf("Calculando... \n");
  //int thread_args[2];

  

  for(int i = 0; i < tam_vet; i++) {
    struct arg_str thread_args;
    thread_args.a = a[i];
    thread_args.b = b[i];
    pthread_create(&threads[i], NULL, func_thread, &thread_args);
  }

  double final_c;
  for(int i = 0; i < tam_vet; i++) {
    pthread_join(threads[i], &final_c);
    printf("DEBUG: %f\n",final_c);
    c[i] = final_c;
    printf("Thread %d encerrada.\n", threads[i]);
  }


  /* soma dos vetores 
  for (i=0; i<tam_vet; i++)
    c[i] = a[i] + b[i];*/

  printf("Terminou!\n");

  if(imprimir)
  {
    /*** imprime os resultados ***/
    printf("******************************************************\n");
    printf("Vetor A:\n");
    for (i=0; i<tam_vet; i++)
      printf("%10.2f  ", a[i]);
    printf("\n");
    printf("******************************************************\n");
    /*** imprime os resultados ***/
    printf("******************************************************\n");
    printf("Vetor C:\n");
    for (i=0; i<tam_vet; i++)
      printf("%10.2f  ", c[i]);
    printf("\n");
    printf("******************************************************\n");
  }

  free(a);
  free(b);
  free(c);
}
