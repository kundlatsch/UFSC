#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int i;

double media(int vetor[], int tamanho){
	double soma_valores = 0;

	for(i = 0; i < tamanho; i++){
		soma_valores += vetor[i];
	}

	return soma_valores / tamanho;
}

double desvio_padrao(int vetor[], int tamanho){
	double soma_dp = 0;
	double media_dp = media(vetor, tamanho);

	for(i = 0; i < tamanho; i++){
		soma_dp += pow((vetor[i] - media_dp), 2.0);
	}	

	return sqrt(soma_dp / tamanho); 
}

int main(int argc, char *argv[]){
	
	int tamanho = atoi(argv[1]);
	
	srand(time(NULL));

	int *vetor = (int*) malloc(tamanho * sizeof(int));

	printf("Numeros do Vetor: \n");
	for(i = 0; i < tamanho; i++){
		vetor[i] = rand() % 100;
		printf(" -> %d \n",vetor[i] );
	}

	printf("Media: %g\n", media(vetor, tamanho));
	printf("Desvio Padrao %g\n", desvio_padrao(vetor, tamanho));

	free(vetor)
	;return 0;
}