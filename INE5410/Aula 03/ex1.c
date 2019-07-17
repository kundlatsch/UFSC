#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

int contador_global = 0;

void *func_thread(void *arg) {
	printf("Thread %d criada.\n", pthread_self());
	int i = 0;
	while(i < 1000 * *(int*)arg) {
		contador_global += 1;
		i++;
		printf("\ncontador_global: %d\n",contador_global);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[ ]) {
	int NUM_THREADS = atoi(argv[1]);
	printf("\nVALOR INICIAL DE contador_global: %d\n---------------------\n\n",contador_global);

	pthread_t threads[NUM_THREADS]; 
	void* retorno;

	for(int i = 0; i < NUM_THREADS; i++) 
		pthread_create(&threads[i], NULL, func_thread, &NUM_THREADS);

	for(int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
		printf("Thread %d encerrada.\n", threads[i]);
	}
	
	printf("\n---------------------\nVALOR FINAL DE contador_global: %d\n",contador_global);

	return 0;
}