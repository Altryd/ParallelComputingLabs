#include <iostream>
#include <chrono>
#include<omp.h>

#define RAND_VALUE 311
int main()
{
    setlocale(LC_ALL, "Ru");
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
    int* matrix_1 = new int[N * N];
    fread(matrix_1, sizeof(int), N * N, Read);
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
    int* matrix_2 = new int[N2 * N2];
    fread(matrix_2, sizeof(int), N2 * N2, Read);
    fclose(Read);

    size_t iteration_all = 8;
    auto begin = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> sum_of_milli = begin - begin;

    for (size_t iteration_count = 0; iteration_count < iteration_all; ++iteration_count)
    {
        //умножение с транспонированием

        begin = std::chrono::steady_clock::now();
        int* matrix_2_transponse = new int[N * N];
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                matrix_2_transponse[i * N + j] = matrix_2[j * N + i];
            }
        }

#ifdef DEBUG
        std::cout << "Результат транспонирования 2ой матрицы" << std::endl;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                std::cout << matrix_2_transponse[i * N + j] << " ";
            }
            std::cout << std::endl;
        }
#endif


        int* matrix_4 = new int[N * N];
        //умножение на трансп.
            #pragma omp parallel for num_threads(16)
            for (int i = 0; i < N; ++i)
            {
                for (int j = 0; j < N; ++j)
                {
                    matrix_4[i * N + j] = 0;
                    for (int k = 0; k < N; ++k)
                    {
                        matrix_4[i * N + j] += matrix_1[i * N + k] * matrix_2_transponse[j * N + k];
                    }
                }
            }

#ifdef DEBUG
        std::cout << "Результат умножения матрицы 1 на транспонированную" << std::endl;
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                std::cout << matrix_4[i * N + j] << " ";
            }
            std::cout << std::endl;
        }
#endif

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> diff = end - begin;
        sum_of_milli += diff;
        std::cout << "Время в  миллисекундах при умножении на транспонированную: " << diff.count() << std::endl;
        if (iteration_count == 0)
        {
            FILE* Write = nullptr;
            fopen_s(&Write, "result.bin", "wb");
            if (Write == NULL)
            {
                printf("error opening file");
                exit(EXIT_FAILURE);
            }
            fwrite(&N, sizeof(int), 1, Write);
            fwrite(matrix_4, sizeof(int), N * N, Write);
            fclose(Write);
        }
        delete[] matrix_2_transponse;
        delete[] matrix_4;
    }
    sum_of_milli /= iteration_all;
    std::cout << "Среднее время в миллисекундах: " << sum_of_milli.count() << std::endl; //8858 для [1500;1500] ,  2638 для [1000; 1000]
    std::cout << "Размерность двух матриц: [" << N << " ; " << N << "]" << std::endl;
    delete[] matrix_1;
    delete[] matrix_2;
    return 0;
}
