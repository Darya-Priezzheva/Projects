#include "../s21_decimal.h"

void s21_bankers_round(s21_decimal *v);
int s21_round_to_scale(s21_decimal *v, int target_scale);
int s21_has_fraction(s21_decimal value);