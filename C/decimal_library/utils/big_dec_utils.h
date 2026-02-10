#include "../s21_decimal.h"

int s21_get_big_bit(s21_big_decimal src, int i);
void s21_set_big_bit(s21_big_decimal *value, int i, int bit);
void s21_zero_big_dec(s21_big_decimal *value);

void s21_shift_left(s21_big_decimal *value, int num);
void s21_shift_right(s21_big_decimal *value, int num);
void s21_dec_to_big(s21_decimal value_1, s21_big_decimal *value_2);
void s21_big_to_dec(s21_decimal *value_1, s21_big_decimal value_2);
int s21_normalization(s21_big_decimal *result, int scale);
void s21_increase_scale_big_dec(s21_big_decimal *value, int n);
void s21_decreace_scale_big_dec(s21_big_decimal *value, int n);
void s21_highest_bit_big_dec(s21_big_decimal v1, s21_big_decimal v2, int *bit_1,
                             int *bit_2);
int s21_big_dec_not_empty(s21_big_decimal value);
void s21_equ_bits_big_dec(s21_big_decimal *value_1, s21_big_decimal *value_2);
int s21_greater_equal_big_dec(s21_big_decimal value_1, s21_big_decimal value_2);
int s21_greater_big_dec(s21_big_decimal value_1, s21_big_decimal value_2);