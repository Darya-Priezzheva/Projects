#include "core_utils.h"

#include "convert_utils.h"
#include "rounding.h"

static inline uint32_t u32(int x) { return (uint32_t)x; }
static inline int i32(uint32_t x) { return (int)x; }

// coryting from core/helpers/set_get.c
s21_decimal s21_set_zero() {
  s21_decimal zero = {{0, 0, 0, 0}};
  // zero.bits[0] = zero.bits[1] = zero.bits[2] = zero.bits[3] = 0;
  s21_set_sign(&zero, 0);
  return zero;
}

// peevesxa
// побитные сеттеры геттеры
void s21_set_bit(s21_decimal *num, int bit, int sign) {
  if (sign == 0) {
    num->bits[bit / 32] = num->bits[bit / 32] & ~(1 << bit % 32);
  } else {
    num->bits[bit / 32] = num->bits[bit / 32] | (1 << bit % 32);
  }
}

int s21_get_bit(s21_decimal num, int bit) {
  int sign;
  if ((num.bits[bit / 32] & (1 << bit % 32)) == 0) {
    sign = 0;
  } else {
    sign = 1;
  }

  return sign;
}

// asougino
//  геттеры и сеттеры скейла и знака
int s21_get_scale(s21_decimal v) { return (v.bits[3] >> 16) & 0xFF; }

void s21_set_scale(s21_decimal *v, int scale) {
  int sign = s21_get_sign(*v);
  v->bits[3] = (sign << 31) | ((scale & 0xFF) << 16);
}

int s21_get_sign(s21_decimal v) { return (v.bits[3] >> 31) & 1; }

void s21_set_sign(s21_decimal *v, int sign) {
  int scale = s21_get_scale(*v);
  v->bits[3] = (sign ? 0x80000000 : 0) | ((scale & 0xFF) << 16);
}

int s21_is_zero(s21_decimal v) {
  return v.bits[0] == 0 && v.bits[1] == 0 && v.bits[2] == 0;
}

// asougino
//  96битное прибавление 1 (мантисса, без знака) и возвращаю carry (0/1)
uint32_t s21_add1_96(s21_decimal *v) {
  uint64_t sum = (uint64_t)u32(v->bits[0]) + 1u;
  v->bits[0] = i32((uint32_t)sum);
  uint32_t carry = (uint32_t)(sum >> 32);

  sum = (uint64_t)u32(v->bits[1]) + carry;
  v->bits[1] = i32((uint32_t)sum);
  carry = (uint32_t)(sum >> 32);

  sum = (uint64_t)u32(v->bits[2]) + carry;
  v->bits[2] = i32((uint32_t)sum);
  carry = (uint32_t)(sum >> 32);
  return carry;
}

// asougino
//  96битное умножение мантиссы на 10. Возвращаю 0=OK, 1=overflow
static int mul10_96(s21_decimal *v) {
  uint64_t t, carry = 0;

  t = (uint64_t)u32(v->bits[0]) * 10u + carry;
  v->bits[0] = i32((uint32_t)t);
  carry = t >> 32;

  t = (uint64_t)u32(v->bits[1]) * 10u + carry;
  v->bits[1] = i32((uint32_t)t);
  carry = t >> 32;

  t = (uint64_t)u32(v->bits[2]) * 10u + carry;
  v->bits[2] = i32((uint32_t)t);
  carry = t >> 32;

  return carry ? 1 : 0;
}

// asougino
//  функция выравнивания
int s21_align_scales(s21_decimal *a, s21_decimal *b) {
  if (!a || !b) return S21_ALIGN_ERROR;

  int sa = s21_get_scale(*a);
  int sb = s21_get_scale(*b);
  if (sa == sb) return S21_ALIGN_OK;

  cu_trim_trailing_zeros(a);
  cu_trim_trailing_zeros(b);
  sa = s21_get_scale(*a);
  sb = s21_get_scale(*b);
  if (sa == sb) return S21_ALIGN_OK;

  int rounded = 0;

  if (sa < sb) {
    while (sa < sb) {
      s21_decimal tmp = *a;
      if (mul10_96(&tmp)) break;
      *a = tmp;
      sa++;
      s21_set_scale(a, sa);
    }
  } else {
    while (sb < sa) {
      s21_decimal tmp = *b;
      if (mul10_96(&tmp)) break;
      *b = tmp;
      sb++;
      s21_set_scale(b, sb);
    }
  }
  if (sa == sb) return rounded ? S21_ALIGN_ROUNDED : S21_ALIGN_OK;

  if (sa < sb) {
    int steps = sb - sa;
    for (int i = 0; i < steps; ++i) {
      s21_bankers_round(b);
    }
    sb = s21_get_scale(*b);
  } else {
    int steps = sa - sb;
    for (int i = 0; i < steps; ++i) {
      s21_bankers_round(a);
    }
    sa = s21_get_scale(*a);
  }
  rounded = 1;

  return (sa == sb) ? (rounded ? S21_ALIGN_ROUNDED : S21_ALIGN_OK)
                    : S21_ALIGN_ERROR;
}