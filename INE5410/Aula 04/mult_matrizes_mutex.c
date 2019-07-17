#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

void *multiplicar_thread(void *arg);
void gerar_matrizes();
void imprimir_matriz(FILE *arquivo, int **matriz);
void imprimir_matrizes();
void liberar_matrizes();


int **matriz1;
int **matriz2;
int **resultado;
int tamanho_matriz;
int linha_atual, coluna_atual;

pthread_mutex_t matrix_mutex;

//Você deve tornar essa função segura para ser executada por mais de uma
//thread ao mesmo tempo. Para isso você deve usar um mutex para sincronizar
//o acesso às variáveis globais. O mutex deve ser iniciado e destruído na
//função main().
//Você deve considerar onde usá-lo na função abaixo e fazer as alterações
//que forem necessárias para que a função execute adequadamente. Você pode
//precisar declarar novas variáveis e adicionar outras verificações de
//término do laço para isso...
void *multiplicar_thread(void *arg) {
    int i;
    int minha_linha, minha_coluna;

    while (linha_atual < tamanho_matriz) {
        pthread_mutex_lock(&matrix_mutex);
        minha_linha = linha_atual;
        minha_coluna = coluna_atual;

        coluna_atual += 1;
        if (coluna_atual >= tamanho_matriz) {
            coluna_atual = 0;
            linha_atual += 1;
        }
        pthread_mutex_unlock(&matrix_mutex);

        if (minha_linha >= tamanho_matriz)
            break;

        for (i = 0; i < tamanho_matriz; i++) {
            resultado[minha_linha][minha_coluna] += matriz1[minha_linha][i] * matriz2[i][minha_coluna];
        }
    }
}

void main(int argc, char* argv[]) {
    //Verifica se recebemos os argumentos necessários
    if (argc < 3) {
        printf("Use: %s [tamanho da matriz] [threads]\n", argv[0]);
        return;
    }

    tamanho_matriz = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    linha_atual = 0;
    coluna_atual = 0;

    //Aloca a memória das matrizes e já gera os números aleatórios das
    //matrizes 1 e 2.
    gerar_matrizes();

    pthread_mutex_init(&matrix_mutex, NULL);
    int i;
    //Crias as threads
    pthread_t threads[num_threads];
    for (i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, multiplicar_thread, NULL);
    }

    //Aguarda elas terminarem...
    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&matrix_mutex);
    printf("imprimindo...\n");
    //Imprime as matrizes e o resultado calculado
    imprimir_matrizes();

    //Libera a memória das matrizes
    liberar_matrizes();
}

/*****************
 Abaixo estão as funções auxiliares. Você não vai precisar fazer alterações
 nestas funções.
*/

//Gera as matrizes usando números aleatórios
void gerar_matrizes() {
    srand(time(NULL));

    matriz1 = malloc(tamanho_matriz * sizeof(int *));
    matriz2 = malloc(tamanho_matriz * sizeof(int *));
    resultado = malloc(tamanho_matriz * sizeof(int *));

    int i, j;
    for (i = 0; i < tamanho_matriz; i++) {
        matriz1[i] = malloc(tamanho_matriz * sizeof(int));
        matriz2[i] = malloc(tamanho_matriz * sizeof(int));
        resultado[i] = malloc(tamanho_matriz * sizeof(int));
        for (j = 0; j < tamanho_matriz; j++) {
            matriz1[i][j] = rand()%10;
            matriz2[i][j] = rand()%10;
            resultado[i][j] = 0;
        }
    }
}

//Imprime os valores de uma matriz no arquivo
void imprimir_matriz(FILE *arquivo, int **matriz) {
    int i, j;
    for (i = 0; i < tamanho_matriz; i++) {
        for (j = 0; j < tamanho_matriz; j++) {
            fprintf(arquivo, "%d ", matriz[i][j]);
        }
        fprintf(arquivo, "\n");
    }
    fprintf(arquivo, "\n");
}

//Imprime todas as matrizes no arquivo de resultados
void imprimir_matrizes() {
    FILE *arquivo;
    arquivo = fopen("resultado.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de resultados!\n");
        exit(1);
    }

    fprintf(arquivo, "Matriz 1\n");
    imprimir_matriz(arquivo, matriz1);
    fprintf(arquivo, "Matriz 2\n");
    imprimir_matriz(arquivo, matriz2);
    fprintf(arquivo, "Resultado 2\n");
    imprimir_matriz(arquivo, resultado);
    fclose(arquivo);
}

//Libera a memória alocada para as matrizes
void liberar_matrizes() {
    int i;
    for (i = 0; i < tamanho_matriz; i++) {
        free(matriz1[i]);
        free(matriz2[i]);
        free(resultado[i]);
    }

    free(matriz1);
    free(matriz2);
    free(resultado);
}
