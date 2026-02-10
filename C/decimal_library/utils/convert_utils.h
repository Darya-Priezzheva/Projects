#include "../s21_decimal.h"

// Конвертаторы для арифметики
s21_decimal s21_decimal_from_char(char c);            // peevesxa READY
int get_exp_from_str(char *str_float);                // peevesxa READY
s21_decimal s21_ten_pow(int pows);                    // peevesxa READY
s21_decimal from_string_to_decimal(char *str_float);  // peevesxa READY

// деление 96-битной мантиссы на 10, вернуть остаток 0..9
uint32_t cu_div10_96(s21_decimal *v);
// сжимает конечные нули десятичной записи (уменьшая scale, без
// округления)
void cu_trim_trailing_zeros(s21_decimal *v);