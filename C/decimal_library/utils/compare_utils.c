#include "compare_utils.h"

// -- 192-битные внутренние помощники — ТОЛЬКО для сравнения
static void big_from_dec(const s21_decimal *d, uint32_t out[6]) {
  out[0] = (uint32_t)d->bits[0];
  out[1] = (uint32_t)d->bits[1];
  out[2] = (uint32_t)d->bits[2];
  out[3] = out[4] = out[5] = 0u;
}

static void big_mul10(uint32_t x[6]) {
  uint64_t carry = 0;
  for (int i = 0; i < 6; ++i) {
    uint64_t t = (uint64_t)x[i] * 10u + carry;
    x[i] = (uint32_t)t;
    carry = t >> 32;
  }
  // 192 бита достаточно для 96битной мантиссы * 10^28
}

static void big_pow10(uint32_t x[6], int k) {
  for (int i = 0; i < k; ++i) big_mul10(x);
}

static int big_cmp(const uint32_t A[6], const uint32_t B[6]) {
  for (int i = 5; i >= 0; --i) {
    if (A[i] != B[i]) return (A[i] < B[i]) ? -1 : 1;
  }
  return 0;
}

int cu_cmp_abs_decimal(const s21_decimal *a, const s21_decimal *b) {
  if (s21_is_zero(*a) && s21_is_zero(*b)) return 0;
  if (s21_is_zero(*a)) return -1;
  if (s21_is_zero(*b)) return 1;

  int sa = s21_get_scale(*a);
  int sb = s21_get_scale(*b);

  uint32_t A[6], B[6];
  big_from_dec(a, A);
  big_from_dec(b, B);
  if (sb > 0) big_pow10(A, sb);
  if (sa > 0) big_pow10(B, sa);
  return big_cmp(A, B);
}
