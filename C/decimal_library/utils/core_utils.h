#include "../s21_decimal.h"

// coryting from core/helpers/set_get.c
s21_decimal s21_set_zero();
void s21_set_bit(s21_decimal *num, int bit, int sign);
int s21_get_bit(s21_decimal num, int bit);

// получить установить знак или скейл
int s21_get_sign(s21_decimal v);  // получить 1 или 0 asougino ready
void s21_set_sign(s21_decimal *v,
                  int sign);  // установить значение от 0 до 1 asougino ready

// прибавить 1 к мантиссе
uint32_t s21_add1_96(s21_decimal *v);

int s21_get_scale(s21_decimal v);  // получить от 0 до 28 asougino ready
void s21_set_scale(s21_decimal *v,
                   int scale);   // нормализовать биты 16..23 asougino ready
int s21_is_zero(s21_decimal v);  // 1 если мантисса == 0 asougino ready

// нормализация scale для двух чисел
int s21_align_scales(
    s21_decimal *a,
    s21_decimal *b);  // выравнивание или расширяет меньший scale, стараясь не
                      // переполнить  asougino READY