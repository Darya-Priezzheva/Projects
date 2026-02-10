#ifndef S21_MATRIX_H
#define S21_MATRIX_H

#define S21_OK 0
#define S21_INCORRECT_MATRIX 1
#define S21_CALC_ERROR 2

#define SUCCESS 1
#define FAILURE 0

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_transpose(matrix_t *A, matrix_t *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, double *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

int s21_check_matrix(matrix_t A, matrix_t B);
int s21_is_incorrect_matrix(matrix_t A);
int s21_is_equal_size(matrix_t A, matrix_t B);
int s21_check_double(double m_cell_value);
int s21_check_mult_size(matrix_t A, matrix_t B);
int s21_check_for_mult(matrix_t A, matrix_t B);
int s21_is_square(matrix_t A);
int s21_is_nan_or_inf(matrix_t A);
void s21_copy_matrix(matrix_t src, matrix_t *dst);
void s21_create_minor_matrix(matrix_t *tmp, matrix_t *A, int i, int j);

#endif