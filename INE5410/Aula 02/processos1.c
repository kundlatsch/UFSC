#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(void) {
	int i = 0;
	int n_process = 2;

	int pid[n_process];
	
	while(i < n_process) {
		pid[i] = fork();

		if (pid[i] > 0) 
			printf("Processo pai criou %d\n", pid[i]);
		
		else if (pid[i] == 0) { 
			printf("Processo filho %d criado\n", getpid());
			break;
		}

		else
			printf("Erro\n");	
		i++;
	}

	while( (wait(NULL)) > 0 );

	return 0;
}