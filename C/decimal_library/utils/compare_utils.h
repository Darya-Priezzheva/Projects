#include "../s21_decimal.h"
#include "core_utils.h"

// мантисса == 0 (без учёта scale/sign)
static inline int cu_dec_is_zero(s21_decimal v) { return s21_is_zero(v); }

// нормализация нуля: -0 то +0 (для корректных сравнений)
static inline void cu_dec_canon_zero(s21_decimal *v) {
  if (s21_is_zero(*v)) s21_set_sign(v, 0);
}
// реализовано как сравнение a*10^sb с b*10^sa во внутренних 192 битах.
int cu_cmp_abs_decimal(const s21_decimal *a, const s21_decimal *b);
