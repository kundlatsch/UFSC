 #include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int i;
int pid[2];

void process_1() {
	i = 0;
	while(i < 2) {

		pid[i] = fork();

		if (pid[i] > 0) 
			printf("Processo pai criou %d\n", pid[i]);

		else if (pid[i] == 0) {
			printf("Processo %d filho de %d\n", getpid(), getppid());
			process_2();
			printf("Processo %d finalizado\n", getpid());
			break;
		}

		else
			printf("Erro\n");


		i++;
	}
	while( (wait(NULL)) > 0 );

}

void process_2() {
	i = 0;
	while(i < 3) {

		int pid_ = fork();
		if (pid_ > 0) {

		}
		else if (pid_ == 0) {
			printf("Processo %d filho de %d\n", getpid(), getppid());

			execl("/bin/date", "date", "+%H:%M:%S:%N", NULL);

			printf("Processo %d finalizado\n", getpid());
			break;
		}

		else
			printf("Erro\n");

		
		sleep(1);
		
		i++;
		
	}

	while( (wait(NULL)) > 0 );	
}

int main(void) {

	process_1();
	return 0;
}