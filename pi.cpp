#include <stdio.h>
#include <iostream>
#include "mpi.h"

int main(int argc, char *argv[]) {
    double start, end;
    int n = 100000;
    int myid, numproc, i;
    double mypi, pi, h, sum, x;
    {
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numproc);
        MPI_Comm_rank(MPI_COMM_WORLD, &myid);
        start = MPI_Wtime();
        h = 1.0 / double(n);
        sum = 0.0;
        for (int i = myid + 1; i <= n; i += numproc) {
            x = h * ((double) i - 0.5);
            sum += (4.0 / (1.0 + x * x));
        }
        mypi = h * sum;
        MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        if (myid == 0) printf("pi is approximately %.16f", pi);
        std::cout << std::endl;
        end = MPI_Wtime();
        MPI_Finalize();
    }
    std::cout << "lead time for process " << myid << " is : " << end - start << std::endl;
    return 0;
}
