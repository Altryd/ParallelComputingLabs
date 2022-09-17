#include <iostream>
#include <chrono>

#define RAND_VALUE 350
int main()
{
    int N = 1001;
    setlocale(LC_ALL, "Ru");
    int* matrix_1 = new int[N * N];
#ifdef DEBUG
    std::cout << "Первая матрица:" << std::endl;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            matrix_1[i * N + j] = rand() % RAND_VALUE;
            std::cout << matrix_1[i * N + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
#else
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            matrix_1[i * N + j] = rand() % RAND_VALUE;
        }
    }
#endif
    FILE* file = nullptr;
    fopen_s(&file, "matrix1.bin", "wb");
    fwrite(&N, sizeof(int), 1, file);
    fwrite(matrix_1, sizeof(int), N*N, file);
    fclose(file);

    int* matrix_2 = new int[N * N];

#ifdef DEBUG
    std::cout << "Вторая матрица:" << std::endl;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            matrix_2[i * N + j] = rand() % RAND_VALUE;
            std::cout << matrix_2[i * N + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
#else
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            matrix_2[i * N + j] = rand() % RAND_VALUE;
        }
    }
#endif
    fopen_s(&file, "matrix2.bin", "wb");
    fwrite(&N, sizeof(int), 1, file);
    fwrite(matrix_2, sizeof(int), N*N, file);
    fclose(file);
}