#include<mpi.h>
#include<cstdlib>


int ProcNum;
int ProcRank;
int Size = 512;
double *A;
double *B;
double *C;


//------------------------------------------------------------
void fillMatrix(double *pMatrix, int Size) {
    for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++) pMatrix[i * Size + j] = rand() % 10;
    }
}

//-------------------------------------------------
void InitProcess(double *&A, double *&B, double *&C) {

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    MPI_Bcast(&Size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (ProcRank == 0) {
        A = new double[Size * Size];
        B = new double[Size * Size];
        C = new double[Size * Size];
        fillMatrix(A, Size);
        fillMatrix(B, Size);
    }
}

void Transp(double *&B, int dim) {
    double temp = 0.0;
    for (int i = 0; i < dim; i++) {
        for (int j = i + 1; j < dim; j++) {
            temp = B[i * dim + j];
            B[i * dim + j] = B[j * dim + i];
            B[j * dim + i] = temp;
        }
    }
}

void MatrixMultiplicationMPI(double *&A, double *&B, double *&C, int &Size) {
    int dim = Size;
    int i, j, k, p, ind;
    double temp;
    MPI_Status Status;
    int ProcPartSize = dim / ProcNum;
    int ProcPartElem = ProcPartSize * dim;
    double *bufA = new double[dim * ProcPartSize];
    double *bufB = new double[dim * ProcPartSize];
    double *bufC = new double[dim * ProcPartSize];
    int ProcPart = dim / ProcNum;
    int part = ProcPart * dim;
    if (ProcRank == 0) {
        Transp(B, Size);
    }

    MPI_Scatter(A, part, MPI_DOUBLE, bufA, part, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, part, MPI_DOUBLE, bufB, part, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    temp = 0.0;
    for (i = 0; i < ProcPartSize; i++) {
        for (j = 0; j < ProcPartSize; j++) {
            for (k = 0; k < dim; k++) temp += bufA[i * dim + k] * bufB[j * dim + k];
            bufC[i * dim + j + ProcPartSize * ProcRank] = temp;
            temp = 0.0;
        }
    }

    int NextProc;
    int PrevProc;
    for (p = 1; p < ProcNum; p++) {
        NextProc = ProcRank + 1;
        if (ProcRank == ProcNum - 1) NextProc = 0;
        PrevProc = ProcRank - 1;
        if (ProcRank == 0) PrevProc = ProcNum - 1;
        MPI_Sendrecv_replace(bufB, part, MPI_DOUBLE, NextProc, 0, PrevProc, 0, MPI_COMM_WORLD, &Status);
        temp = 0.0;
        for (i = 0; i < ProcPartSize; i++) {
            for (j = 0; j < ProcPartSize; j++) {
                for (k = 0; k < dim; k++) {
                    temp += bufA[i * dim + k] * bufB[j * dim + k];
                }
                if (ProcRank - p >= 0)
                    ind = ProcRank - p;
                else ind = (ProcNum - p + ProcRank);
                bufC[i * dim + j + ind * ProcPartSize] = temp;
                temp = 0.0;
            }
        }
    }
    MPI_Gather(bufC, ProcPartElem, MPI_DOUBLE, C, ProcPartElem, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    delete[]bufA;
    delete[]bufB;
    delete[]bufC;
}


int main(int argc, char *argv[]) {
    double beg, end;
    MPI_Init(&argc, &argv);
    InitProcess(A, B, C);
    beg = MPI_Wtime();
    MatrixMultiplicationMPI(A, B, C, Size);
    end = MPI_Wtime();
    double time = end - beg;
    if (ProcRank == 0) {
        printf("\nRuntime for size 512: ");
        printf("%7.4f", time);
        printf("\n");
    }
    MPI_Finalize();
    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}

// mpic++ main.cpp -o main

// mpiexec -n 4 ./main