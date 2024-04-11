#include "s21_matrix.h"

// void print_matrix(matrix_t *A) {
//     for (int i = 0; i < A->rows; i++) {
//         for (int j = 0; j < A->columns; j++) {
//             printf("%-10f", A->matrix[i][j]);
//         }
//         printf("\n");
//     }
// }

int check_matrix(matrix_t matrix) {
    int answer = 0;
    if (matrix.columns <= 0 || matrix.rows <= 0) { answer = 1;}
    return answer;
}

/*
Все операции (кроме сравнения матриц) должны возвращать результирующий код:
0 - OK
1 - Ошибка, некорректная матрица
2 - Ошибка вычисления (несовпадающие размеры матриц; матрица,
для которой нельзя провести вычисления и т.д.)
*/


// Проверенно
int s21_create_matrix(int rows, int columns, matrix_t *result) {
    int answer = 0;
    if ((rows >= 0 && columns >= 0) && result != NULL) {
        result->rows = rows;
        result->columns = columns;
        result->matrix = (double**)calloc(rows, sizeof(double));
        for (int i = 0; i < rows; i++) {
            result->matrix[i] = (double*)calloc(columns, sizeof(double));
        }
    } else {
        answer = 1;
    }
    return answer;
}

// Проверенно
void s21_remove_matrix(matrix_t *A) {
    if (A != NULL) {
        for (int i = 0; i < A->rows; i++) {
            free(A->matrix[i]);
        }
        free(A->matrix);
    }
}

// Сравнение до 6 знака
// Проверенно
int s21_eq_matrix(matrix_t *A, matrix_t *B) {
    int answer = 1;
    if (!check_matrix(*A) && !check_matrix(*B)) {
        int n = A->rows, m = A->columns;
        if ((n == B->rows) && (m == B->columns)) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    double a = round(A->matrix[i][j] * 1000000) / 1000000;
                    double b = round(B->matrix[i][j] * 1000000) / 1000000;
                    if (a != b) {
                        answer = 0;
                        break;
                    }
                }
            }
        } else {
            answer = 0;
        }
    } else {
        answer = 0;
    }
    return answer;
}

// Проверенно
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int answer = 0;
    if (!check_matrix(*A) && !check_matrix(*B)) {
        int n = A->rows, m = A->columns;
        if ((n == B->rows) && (m == B->columns)) {
            s21_create_matrix(n, m, result);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    double a = round(A->matrix[i][j] * 1000000) / 1000000;
                    double b = round(B->matrix[i][j] * 1000000) / 1000000;
                    result->matrix[i][j]= a + b;
                }
            }
        } else {
            answer = 2;
        }
    } else {
        answer = 1;
    }
    return answer;
}

// Проверенно
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int answer = 0;
    if (!check_matrix(*A) && !check_matrix(*B)) {
        int n = A->rows, m = A->columns;
        if ((n == B->rows) && (m == B->columns)) {
            s21_create_matrix(n, m, result);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    double a = round(A->matrix[i][j] * 1000000) / 1000000;
                    double b = round(B->matrix[i][j] * 1000000) / 1000000;
                    result->matrix[i][j]= a - b;
                }
            }
        } else {
            answer = 2;
        }
    } else {
        answer = 1;
    }
    return answer;
}

// Проверенно
int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
    int answer = 0;
    if (!check_matrix(*A)) {
        int n = A->rows, m = A->columns;
        s21_create_matrix(n, m, result);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    double a = round(A->matrix[i][j] * 1000000) / 1000000;
                    result->matrix[i][j]= a * number;
                }
        }
    } else {
        answer = 1;
    }
    return answer;
}

// Проверенно
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int answer = 0;
    if (!check_matrix(*A) && !check_matrix(*B)) {
        if (A->columns == B->rows) {
            s21_create_matrix(A->rows, B->columns, result);
            for (int i = 0; i < A->rows; i++) {
                for (int j = 0; j < B->columns; j++) {
                    result->matrix[i][j] = 0.0;
                    for (int z = 0; z < A->columns; z++) {
                        double a = round(A->matrix[i][z] * 1000000) / 1000000;
                        double b = round(B->matrix[z][j] * 1000000) / 1000000;
                        result->matrix[i][j] += a * b;
                    }
                }
            }
        } else {
            answer = 2;
        }
    } else {
        answer = 1;
    }
    return answer;
}

// Транспонирование матрицы (transpose)
// Проверенно
int s21_transpose(matrix_t *A, matrix_t *result) {
    int answer = 0;
    if (!check_matrix(*A)) {
        s21_create_matrix(A->columns, A->rows, result);
        for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
                result->matrix[j][i] = A->matrix[i][j];
            }
        }
    } else {
        answer = 1;
    }
    return answer;
}

// Минор матрицы и матрица алгебраических дополнений (calc_complements)
// Проверенно
int s21_determinant(matrix_t *A, double *result) {
    int answer = check_matrix(*A);
    *result = 0;
    if (!answer) {
        if (A->columns == A->rows) {
            if (A->columns == 1) {
                *result = A->matrix[0][0];
            } else if (A->columns == 2) {
                *result = A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
            } else {
                double temp_det = 0;
                for (int k = 0; k < A->rows; k++) {
                    matrix_t temp;
                    s21_create_matrix(A->rows - 1, A->columns - 1, &temp);
                    s21_minor(A, 0, k, &temp);
                    s21_determinant(&temp, result);
                    temp_det += pow(-1, k) * A->matrix[0][k] * (*result);
                    s21_remove_matrix(&temp);
                }
                *result = round(temp_det * 1000000) / 1000000;
            }
        } else {
            answer = 2;
        }
    }
    return answer;
}

// Проверенно
void s21_minor(matrix_t *A, int rows, int column, matrix_t *temp) {
    for (int i = 0, a = 0; i < A->columns; i++) {
        if (i == rows) continue;
        for (int j = 0, b = 0; j < A->columns; j++) {
            if (j == column) continue;
            temp->matrix[a][b] = A->matrix[i][j];
            b++;
        }
        a++;
    }
}

// Обратная матрица (inverse_matrix)
// Проверенно
int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
    int answer = 0;
    if (!check_matrix(*A)) {
        double det = 0;
        int flag = s21_determinant(A, &det);
        if (flag == 0 && fabs(det) > 1e-7 && A->columns == A->rows) {
            s21_create_matrix(A->rows, A->columns, result);
            matrix_t trans; matrix_t calc;
            s21_calc_complements(A, &calc);
            s21_transpose(&calc, &trans);
            s21_remove_matrix(&calc);
            s21_mult_number(&trans, (1. / det), result);
            s21_remove_matrix(&trans);
        } else {
            answer = 2;
        }
    } else {
        answer = 1;
    }
    return answer;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
    int answer = 0;
    if (!check_matrix(*A)) {
        if (A->columns == A->rows) {
            s21_create_matrix(A->rows, A->columns, result);
            for (int i = 0; i < A->rows; i++) {
                for (int j = 0; j < A->columns; j++) {
                    matrix_t minor;
                    s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
                    s21_minor(A, i, j, &minor);
                    int otr = pow(-1, (i + j));
                    double det; s21_determinant(&minor, &det);
                    result->matrix[i][j] = det * otr;
                    s21_remove_matrix(&minor);
                }
            }
        } else {
            answer = 2;
        }
    } else {
        answer = 1;
    }
    return answer;
}
