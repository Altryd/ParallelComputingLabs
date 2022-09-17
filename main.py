import numpy as np


if __name__ == '__main__':
    with open(r"C:\Users\Altryd\source\repos\Parallel_prog\Parallel_prog\matrix1.bin", "rb") as fp:
        N = fp.read(4)
        N = int.from_bytes(N, byteorder='little')
        X = np.fromfile(fp, dtype='int', count=N*N)
        X = np.reshape(X, newshape=(N, N))
    with open(r"C:\Users\Altryd\source\repos\Parallel_prog\Parallel_prog\matrix2.bin", "rb") as fp:
        N2 = fp.read(4)
        N2 = int.from_bytes(N2, byteorder='little')
        assert N == N2
        Y = np.fromfile(fp, dtype='int', count=N*N)
        Y = np.reshape(Y, newshape=(N,N))

    Z = np.matmul(X, Y)
    with open(r"C:\Users\Altryd\source\repos\Parallel_prog\Parallel_prog\result.bin", "rb") as fp:
        N3 = fp.read(4)
        N3 = int.from_bytes(N3, byteorder='little')
        assert N3 == N2
        result = np.fromfile(fp, dtype='int', count=N * N)
        result = np.reshape(result, newshape=(N, N))
        # result[0][0] = -2
    print(Z)
    print(result)
    print("Размеры выходной матрицы: {}".format(result.shape))
    print("Равны" if np.array_equal(Z, result) else "НЕ равны")
