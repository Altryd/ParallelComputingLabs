#include <iostream>
#include <chrono>
#include <sstream>

#define RAND_VALUE 350
int main(int argc, char* argv[])
{

    int N = 0;
    if (argc < 2) N = 320;
    else {
        std::istringstream ss(argv[1]);
        if (!(ss >> N))
        {
            std::cerr << "Invalid Number: " << argv[1] << std::endl;
            return 0;
        }
        std::cout << "N is: " << N << std::endl;
    }
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