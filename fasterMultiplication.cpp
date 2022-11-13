#include <iostream>
#include <chrono>
#include <mpi.h>
#include <stdio.h>
#include <string>


#define RAND_VALUE 311
int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");
    double all_time = 0.0;
    double startwtime = 0.0, endwtime;
    MPI_Init(&argc, &argv);
    int my_rank, proc_num;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // Get the rank of the process
    int dimension = 0;
    int* matrix_1 = nullptr, *matrix_2_transponse = nullptr, *matrix_4 = nullptr;

    if (my_rank == 0)
    {
        FILE* Read = NULL;
        fopen_s(&Read, "matrix1.bin", "rb");
        if (Read == NULL)
        {
            printf("error opening file");
            exit(EXIT_FAILURE);
        }
        int N = 0;
        fread(&N, sizeof(int), 1, Read);
        if (Read == NULL)
        {
            printf("error opening file");
            exit(EXIT_FAILURE);
        }
        matrix_1 = new int[N * N];
        fread(matrix_1, sizeof(int), N * N, Read);
#ifdef DEBUG
        std::cout << "Matrix 1 readed:" << std::endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j)
                std::cout << matrix_1[i * N + j] << "   ";
            std::cout << std::endl;
        }
#endif
        fclose(Read);

        fopen_s(&Read, "matrix2.bin", "rb");
        if (Read == NULL)
        {
            printf("error opening file");
            exit(EXIT_FAILURE);
        }
        int N2 = 0;
        fread(&N2, sizeof(int), 1, Read);
        if (N2 != N || Read == NULL)
        {
            exit(EXIT_FAILURE);
        }
        if (N % proc_num != 0)
        {
            std::cout << "N �� ������� �� ���������� ���������, �������� �� ����������!" << std::endl;
            return 0;
        }
        int* matrix_2 = new int[N2 * N2];
        fread(matrix_2, sizeof(int), N2 * N2, Read);
        fclose(Read);
        startwtime = MPI_Wtime();
        matrix_2_transponse = new int[N * N];
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                matrix_2_transponse[i * N + j] = matrix_2[j * N + i];
            }
        }
        delete[] matrix_2;
        matrix_2 = nullptr;
#ifdef DEBUG
        std::cout << "Matrix 2 transposed:" << std::endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j)
                std::cout << matrix_2_transponse[i * N + j] << "   ";
            std::cout << std::endl;
        }
#endif
        matrix_4 = new int[N * N];
        dimension = N;
    }
    MPI_Bcast(&dimension, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (my_rank)
    {
        matrix_2_transponse = new int[dimension * dimension];
    }
    int local_size = dimension / proc_num;
    int* local_a_part = new int[local_size * dimension]; 
    int* local_result = new int[local_size * dimension];
    MPI_Scatter(matrix_1 + my_rank * dimension * local_size, dimension * local_size, MPI_INT, local_a_part, dimension * local_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrix_2_transponse, dimension * dimension, MPI_INT, 0, MPI_COMM_WORLD);
#ifdef DEBUG
    std::string row;
    for (int i = 0; i < local_size; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            row += std::to_string(local_a_part[i * dimension + j]) + std::string("\t");
        }
        row += std::string("\n");
    }
    std::cout << "--------------\n"  << "my rank is " << my_rank << "  my rows in matrix_1:" << row << std::endl;

    row.clear();
    for (int i = 0; i < local_size; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            row += std::to_string(matrix_2_transponse[i * dimension + j]) + std::string("\t");
        }
        row += std::string("\n");
    }
    std::cout << "--------------\n" << "my rank is " << my_rank << "  my rows in matrix_2_transponse:" << row << std::endl;

    


    printf("Hello World! My rank is %d, number of process: %d, my dimension: %d\n", my_rank, proc_num, dimension);
#endif
	for (int i = 0; i < local_size; ++i)
	{
		for (int j = 0; j < dimension; ++j)
		{
			local_result[i * dimension + j] = 0;
			for (int k = 0; k < dimension; ++k)
			{
				local_result[i * dimension + j] += local_a_part[i * dimension + k] * matrix_2_transponse[j * dimension + k];
			}
		}
	}
#ifdef DEBUG
    for (int i = 0; i < local_size; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            row += std::to_string(local_result[i * dimension + j]) + std::string("\t");
        }
        row += std::string("\n");
    }
    std::cout << "--------------\n" << "my rank is " << my_rank << "  my rows in local_result:" << row << std::endl;
#endif
     MPI_Gather(local_result, local_size* dimension, MPI_INT, matrix_4, dimension* local_size, MPI_INT, 0, MPI_COMM_WORLD);
     endwtime = MPI_Wtime();
#ifdef DEBUG
    if (my_rank == 0)
    {
        std::cout << "Final Result:" << std::endl;
        for (int i = 0; i < dimension; ++i)
        {
            for (int j = 0; j < dimension; ++j)
            {
                std::cout << matrix_4[i * dimension + j] << "  ";
            }
            std::cout << std::endl;
        }
    }
#endif
    if (my_rank)
    {
        delete[] matrix_2_transponse;
        delete[] local_result;
        delete[] local_a_part;
    }
    MPI_Finalize();
    delete[] matrix_1;
    delete[] matrix_2_transponse;
	FILE* Write = nullptr;
	fopen_s(&Write, "result.bin", "wb");
	if (Write == NULL)
	{
		printf("error opening file");
		exit(EXIT_FAILURE);
	}
	fwrite(&dimension, sizeof(int), 1, Write);
	fwrite(matrix_4, sizeof(int), dimension* dimension, Write);
	fclose(Write);
    delete[] matrix_4;
    std::cout << "Operation successfully completed" << std::endl;
    std::cout << "N: " << dimension << std::endl;
    std::cout << "Number of Processes: " << proc_num << std::endl;
    std::cout << "Time in millisecond: " << (endwtime - startwtime)*1000  << std::endl;
    return 0;
}
