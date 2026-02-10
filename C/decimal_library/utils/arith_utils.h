#include "../s21_decimal.h"

int s21_div_big_dec(s21_big_decimal value_1, s21_big_decimal value_2,
                    s21_big_decimal *result);
void s21_add_big_dec(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result);
void s21_sub_big_dec(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result);
void s21_mul_big_dec(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result);
int s21_dec_not_empty(s21_decimal value);
