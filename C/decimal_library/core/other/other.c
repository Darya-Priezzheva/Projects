#include "../../s21_decimal.h"
#include "../../utils/convert_utils.h"
#include "../../utils/core_utils.h"
#include "../../utils/rounding.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (!result) return 1;
  int status = s21_truncate(value, result);
  if (status) return status;

  if (s21_get_sign(value) && s21_has_fraction(value)) {
    s21_decimal one = {{1, 0, 0, 0}};
    status = s21_sub(*result, one, result);
  }
  if (status == S21_INF_NEG) status = 1;

  return status;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int status = 0;
  if (!result) return 1;
  int sc = s21_get_scale(value);
  while (sc > 0 && !status) {
    uint32_t d = cu_div10_96(&value);
    sc--;
    if (sc == 0 && d >= 5) {
      status = s21_add1_96(&value);
    }
  }

  s21_set_scale(&value, 0);
  *result = value;
  return status;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (!result) return 1;
  int sc = s21_get_scale(value);
  while (sc-- > 0) (void)cu_div10_96(&value);
  *result = value;
  s21_set_scale(result, 0);
  return 0;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (!result) return 1;
  *result = value;
  s21_set_sign(result, !s21_get_sign(value));
  return 0;
}
