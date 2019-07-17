#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

int contador_global = 0;
pthread_mutex_t mutex;

void *ThreadFunc(void *arg) {
    int numOfLoops = *(int *)arg;
    int i;
    for (i = 0; i < numOfLoops; i++) {
        pthread_mutex_lock(&mutex);
        contador_global += 1;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int numOfThreads = argc >= 2? atoi(argv[1]) : 10;

    int numOfLoops = numOfThreads * 1000;
    
    pthread_t threads[numOfThreads];
        
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < numOfThreads; i++)
        pthread_create(&threads[i], NULL, ThreadFunc, (void*)&numOfLoops);

    for (int i = 0; i < numOfThreads; i++)
        pthread_join(threads[i], NULL);
    
    printf("Processo principal finalizado.\n");
    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", numOfThreads * numOfLoops);

    pthread_mutex_destroy(&mutex);
    
    return 0;
}