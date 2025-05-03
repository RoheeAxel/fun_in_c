import numpy


matrix_size = 1920

matrix1 = numpy.random.rand(matrix_size, matrix_size)
matrix2 = numpy.random.rand(matrix_size, matrix_size)

for i in range(matrix_size):
    for j in range(matrix_size):
        matrix1[i][j] = i + j
        matrix2[i][j] = i + j

result = numpy.dot(matrix1, matrix2)

# print(result)