#ifndef DEC
#define DEC

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  unsigned bits[8];
} s21_big_decimal;

// --Коды возврата для арифметики
#define S21_OK 0           // OK
#define S21_INF_POS 1      // переполнение (overflow plus)
#define S21_INF_NEG 2      // переполнение - (negative too small)
#define S21_DIV_BY_ZERO 3  // деление на ноль

// --Коды возврата для конвертеров
#define S21_CONVERT_ERROR 1  // ошибка конвертации

// --Коды возврата для нормализации скейла
#define S21_ALIGN_OK 0
#define S21_ALIGN_ROUNDED 1
#define S21_ALIGN_ERROR 2

// --Арифметика
int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);

// --Сравнения (1 = TRUE, 0 = FALSE)
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);

// --Конверторы (0 = OK, 1 = ошибка)
int s21_from_int_to_decimal(int src,
                            s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src,
                            int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// --Другие функции (0 = ОК, 1 = ошибка)
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif