#include "rounding.h"

#include "convert_utils.h"
#include "core_utils.h"

// банковское округление на 1 десятичный разряд: scale--
void s21_bankers_round(s21_decimal *v) {
  int sc = s21_get_scale(*v);
  if (sc <= 0) return;

  uint32_t d = cu_div10_96(v);
  sc--;
  s21_set_scale(v, sc);

  if (d > 5) {
    (void)s21_add1_96(v);
  } else if (d == 5) {
    s21_decimal tmp = *v;
    uint32_t last = cu_div10_96(&tmp);
    if (last & 1u) (void)s21_add1_96(v);
  }
}

// округляю до целевого scale (уменьшая точность)
int s21_round_to_scale(s21_decimal *v, int target_scale) {
  int sc = s21_get_scale(*v);
  if (target_scale < 0 || target_scale > sc) return 1;

  while (sc > target_scale) {
    s21_bankers_round(v);
    sc = s21_get_scale(*v);
  }
  cu_trim_trailing_zeros(v);
  return 0;
}

int s21_has_fraction(s21_decimal value) {
  int sc = s21_get_scale(value);
  if (sc == 0) return 0;

  s21_decimal truncated;
  s21_truncate(value, &truncated);
  return !s21_is_equal(truncated, value);
}