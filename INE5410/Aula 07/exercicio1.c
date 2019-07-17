#include <mpi.h>
#include <stdio.h>

#define VALUE 10

int main(int argc, char **argv) {
	int size, rank, message_slave, message_master = {VALUE};
	MPI_Status st;
	
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank == 0) {
		for(int i = 1; i < size; i++) {
			MPI_Send(&message_master, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(&message_master, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
		message_slave = rank + message_master;
		MPI_Send(&message_slave, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	if(rank == 0) {
		for(int i = 1; i < size; i++) {
			MPI_Recv(&message_slave, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
			printf("message_slave: %d\n", message_slave);
		}
	}


	MPI_Finalize();
	
	return 0;
}