#include "../../s21_decimal.h"
#include "../../utils/compare_utils.h"
#include "../../utils/core_utils.h"

int s21_is_equal(s21_decimal a, s21_decimal b) {
  cu_dec_canon_zero(&a);
  cu_dec_canon_zero(&b);
  if (s21_is_zero(a) && s21_is_zero(b)) return 1;
  if (s21_get_sign(a) != s21_get_sign(b)) return 0;
  return cu_cmp_abs_decimal(&a, &b) == 0;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return !s21_is_equal(a, b);
}

int s21_is_less(s21_decimal a, s21_decimal b) {
  cu_dec_canon_zero(&a);
  cu_dec_canon_zero(&b);
  if (s21_is_zero(a) && s21_is_zero(b)) return 0;

  int sa = s21_get_sign(a), sb = s21_get_sign(b);
  if (sa != sb) return sa > sb;  // минус < плюс

  int cmp = cu_cmp_abs_decimal(&a, &b);
  return (sa == 0) ? (cmp < 0) : (cmp > 0);
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_less(a, b) || s21_is_equal(a, b);
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  return !s21_is_less_or_equal(a, b);
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  return !s21_is_less(a, b);
}