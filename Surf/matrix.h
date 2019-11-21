double determinant(double *A, int dim);

/* 전치 행렬을 구하는 함수 */
void transpose_matrix(double *A);

/* 역행렬을 구하는 함수 */
void inverse_matrix(double *A, double det);

void mul_matrix(double* m1, double* m2, double* m3, int row);