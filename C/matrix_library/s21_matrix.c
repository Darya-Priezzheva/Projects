#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int status = S21_OK;
  if (rows < 1 || columns < 1 || result == NULL) {
    status = S21_INCORRECT_MATRIX;
  }
  result->rows = rows;
  result->columns = columns;
  result->matrix = (double **)malloc(rows * sizeof(double *));

  for (int i = 0; i < rows && status == S21_OK; i++) {
    result->matrix[i] = (double *)malloc(columns * sizeof(double));
    if (result->matrix[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(result->matrix[i]);
      }
      free(result->matrix);
      result->matrix = NULL;
      status = S21_INCORRECT_MATRIX;
    }
  }
  if (status == S21_OK) {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        result->matrix[i][j] = 0;
      }
    }
  }

  return status;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < A->rows; ++i) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
  }
  A->rows = 0;
  A->columns = 0;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int status = SUCCESS;

  status = (A->rows == B->rows) && (A->columns == B->columns);
  if (status) {
    for (int i = 0; i < A->rows && status == SUCCESS; i++) {
      for (int j = 0; j < A->columns && status == SUCCESS; j++) {
        status =
            fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-6 ? FAILURE : SUCCESS;
      }
    }
  }
  return status;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (s21_is_incorrect_matrix(*A) || s21_is_incorrect_matrix(*B))
    return S21_INCORRECT_MATRIX;
  int status = s21_check_matrix(*A, *B);
  if (!status) {
    status = s21_create_matrix(A->rows, A->columns, result);
  }
  for (int i = 0; i < A->rows && !status; i++) {
    for (int j = 0; j < A->columns && !status; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      status = s21_check_double(result->matrix[i][j]);
    }
  }

  return status;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (s21_is_incorrect_matrix(*A) || s21_is_incorrect_matrix(*B))
    return S21_INCORRECT_MATRIX;
  int status = s21_check_matrix(*A, *B);
  if (!status) {
    status = s21_create_matrix(A->rows, A->columns, result);
  }
  for (int i = 0; i < A->rows && !status; i++) {
    for (int j = 0; j < A->columns && !status; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      status = s21_check_double(result->matrix[i][j]);
    }
  }
  return status;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  if (s21_is_incorrect_matrix(*A)) return S21_INCORRECT_MATRIX;
  int status = s21_check_double(number);
  if (!status) {
    status = s21_create_matrix(A->rows, A->columns, result);
  }
  for (int i = 0; i < A->rows && !status; i++) {
    for (int j = 0; j < A->columns && !status; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
      status = s21_check_double(result->matrix[i][j]);
    }
  }
  return status;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (s21_is_incorrect_matrix(*A) || s21_is_incorrect_matrix(*B)) {
    return S21_INCORRECT_MATRIX;
  }
  int status = s21_check_for_mult(*A, *B);
  if (!status) {
    status = s21_create_matrix(A->rows, B->columns, result);
  }
  for (int i = 0; i < result->rows && !status; i++) {
    for (int j = 0; j < result->columns && !status; j++) {
      for (int k = 0; k < B->rows; k++) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        status = s21_check_double(result->matrix[i][j]);
      }
    }
  }
  return status;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  if (s21_is_incorrect_matrix(*A)) return S21_INCORRECT_MATRIX;
  int status = s21_create_matrix(A->columns, A->rows, result);
  for (int i = 0; i < result->rows && !status; i++) {
    for (int j = 0; j < result->columns && !status; j++) {
      result->matrix[i][j] = A->matrix[j][i];
      status = s21_check_double(result->matrix[i][j]);
    }
  }

  return status;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int status = s21_is_incorrect_matrix(*A) ? S21_INCORRECT_MATRIX : S21_OK;
  status = (!status && (!s21_is_square(*A) || s21_is_nan_or_inf(*A)))
               ? S21_CALC_ERROR
               : status;
  if (status != S21_OK) return status;

  s21_create_matrix(A->rows, A->columns, result);

  if (A->rows == 1) {
    result->matrix[0][0] = 1.0;
    return S21_OK;
  }

  for (int i = 0; i < A->rows && !status; i++) {
    for (int j = 0; j < A->columns && !status; j++) {
      matrix_t minor;
      s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
      s21_create_minor_matrix(&minor, A, i, j);
      double d = 0.0;
      status = s21_determinant(&minor, &d);
      double sign = (i + j) % 2 == 0 ? 1 : -1;
      result->matrix[i][j] = sign * d;
      s21_remove_matrix(&minor);
    }
  }
  return status;
}

int s21_determinant(matrix_t *A, double *result) {
  int status = s21_is_incorrect_matrix(*A) ? S21_INCORRECT_MATRIX : S21_OK;
  status = (!status && (!s21_is_square(*A) || s21_is_nan_or_inf(*A)))
               ? S21_CALC_ERROR
               : status;
  if (status != S21_OK) return status;

  int n = A->rows;
  double det = 1.0;
  int sign = 1;

  matrix_t tmp = {0};
  if (s21_create_matrix(n, n, &tmp) != S21_OK) return S21_INCORRECT_MATRIX;
  s21_copy_matrix(*A, &tmp);

  for (int i = 0; i < n; i++) {
    int pivot = i;
    for (int k = i + 1; k < n; k++) {
      if (fabs(tmp.matrix[k][i]) > fabs(tmp.matrix[pivot][i])) {
        pivot = k;
      }
    }

    if (fabs(tmp.matrix[pivot][i]) < 1e-6) {
      *result = 0.0;
      s21_remove_matrix(&tmp);
      return S21_OK;
    }

    if (pivot != i) {
      double *swap = tmp.matrix[i];
      tmp.matrix[i] = tmp.matrix[pivot];
      tmp.matrix[pivot] = swap;
      sign *= -1;
    }

    for (int k = i + 1; k < n; k++) {
      double factor = tmp.matrix[k][i] / tmp.matrix[i][i];
      for (int j = i; j < n; j++) tmp.matrix[k][j] -= factor * tmp.matrix[i][j];
    }
  }

  for (int i = 0; i < n; ++i) {
    det *= tmp.matrix[i][i];
  }
  det *= sign;

  *result = det;
  s21_remove_matrix(&tmp);
  return S21_OK;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int status = s21_is_incorrect_matrix(*A) ? S21_INCORRECT_MATRIX : S21_OK;
  status = (!status && (!s21_is_square(*A) || s21_is_nan_or_inf(*A)))
               ? S21_CALC_ERROR
               : status;
  if (status != S21_OK) return status;

  double d = 0.0;
  int n = A->rows;
  status = s21_determinant(A, &d);
  if (status != S21_OK || fabs(d) < 1e-7) status = S21_CALC_ERROR;

  if (!status) {
    if (n == 1) {
      s21_create_matrix(1, 1, result);
      result->matrix[0][0] = 1.0 / A->matrix[0][0];
      return S21_OK;
    }
    matrix_t tmp = {0};
    matrix_t tr = {0};
    status = s21_calc_complements(A, &tmp);
    if (!status) {
      s21_transpose(&tmp, &tr);
      s21_remove_matrix(&tmp);

      s21_create_matrix(n, n, result);

      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          result->matrix[i][j] = tr.matrix[i][j] / d;
        }
      }
      s21_remove_matrix(&tr);
    }
  }
  return status;
}

//==HELPERS==//

int s21_check_matrix(matrix_t A, matrix_t B) {
  int status = (!s21_is_equal_size(A, B)) ? S21_OK : S21_CALC_ERROR;
  return status;
}

int s21_is_incorrect_matrix(matrix_t A) {
  int status = S21_OK;
  status = ((A.matrix != NULL) && (A.rows > 0) && (A.columns > 0))
               ? S21_OK
               : S21_INCORRECT_MATRIX;
  return status;
}

int s21_is_equal_size(matrix_t A, matrix_t B) {
  int status = S21_OK;
  status =
      (A.rows == B.rows && A.columns == B.columns) ? S21_OK : S21_CALC_ERROR;
  return status;
}

int s21_is_nan_or_inf(matrix_t A) {
  int status = S21_OK;
  for (int i = 0; i < A.rows && !status; i++) {
    for (int j = 0; j < A.rows && !status; j++) {
      status = s21_check_double(A.matrix[i][j]);
    }
  }
  return status;
}

int s21_check_double(double m_cell_value) {
  int status = S21_OK;
  if (isnan(m_cell_value) || isinf(m_cell_value)) {
    status = S21_CALC_ERROR;
  }
  return status;
}

int s21_check_for_mult(matrix_t A, matrix_t B) {
  int status = (!s21_check_mult_size(A, B)) ? S21_OK : S21_CALC_ERROR;
  return status;
}

int s21_check_mult_size(matrix_t A, matrix_t B) {
  int status = S21_OK;
  status = (A.columns == B.rows) ? S21_OK : S21_CALC_ERROR;
  return status;
}

int s21_is_square(matrix_t A) { return (A.rows == A.columns && A.rows > 0); }

void s21_copy_matrix(matrix_t src, matrix_t *dst) {
  for (int i = 0; i < src.rows; i++) {
    for (int j = 0; j < src.columns; j++) {
      dst->matrix[i][j] = src.matrix[i][j];
    }
  }
}

void s21_create_minor_matrix(matrix_t *tmp, matrix_t *A, int i, int j) {
  int k = 0, l = 0;
  for (int m = 0; m < A->rows; m++) {
    if (m == i) continue;
    l = 0;
    for (int n = 0; n < A->columns; n++) {
      if (n == j) continue;
      tmp->matrix[k][l] = A->matrix[m][n];
      l++;
    }
    k++;
  }
}
