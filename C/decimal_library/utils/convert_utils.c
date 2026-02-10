#include "convert_utils.h"

#include "../s21_decimal.h"
#include "core_utils.h"

// asougino READY
uint32_t cu_div10_96(s21_decimal *v) {
  uint64_t cur;
  uint32_t rem = 0, q;

  cur = ((uint64_t)rem << 32) | (uint32_t)v->bits[2];
  q = (uint32_t)(cur / 10u);
  rem = (uint32_t)(cur % 10u);
  v->bits[2] = (int)q;

  cur = ((uint64_t)rem << 32) | (uint32_t)v->bits[1];
  q = (uint32_t)(cur / 10u);
  rem = (uint32_t)(cur % 10u);
  v->bits[1] = (int)q;

  cur = ((uint64_t)rem << 32) | (uint32_t)v->bits[0];
  q = (uint32_t)(cur / 10u);
  rem = (uint32_t)(cur % 10u);
  v->bits[0] = (int)q;

  return rem;
}

// asougino READY
void cu_trim_trailing_zeros(s21_decimal *v) {
  int sc = s21_get_scale(*v);
  if (sc <= 0 || s21_is_zero(*v)) return;

  while (sc > 0) {
    s21_decimal tmp = *v;
    uint32_t rem = cu_div10_96(&tmp);
    if (rem == 0) {
      *v = tmp;
      sc--;
    } else {
      break;
    }
  }
  s21_set_scale(v, sc);
}