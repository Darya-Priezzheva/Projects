#include "../../s21_decimal.h"
#include "../../utils/convert_utils.h"
#include "../../utils/core_utils.h"
#include "../../utils/rounding.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) return S21_CONVERT_ERROR;
  *dst = s21_set_zero();

  int sign = (src < 0);
  unsigned int mag =
      (src < 0) ? (unsigned int)(0u - (unsigned int)src) : (unsigned int)src;

  dst->bits[0] = (int)mag;
  s21_set_scale(dst, 0);
  s21_set_sign(dst, sign);
  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (!dst) return S21_CONVERT_ERROR;

  int sc = s21_get_scale(src);
  while (sc-- > 0) (void)cu_div10_96(&src);
  s21_set_scale(&src, 0);

  if (src.bits[1] != 0 || src.bits[2] != 0) return S21_CONVERT_ERROR;
  uint32_t mag = (uint32_t)src.bits[0];
  int sign = s21_get_sign(src);

  if (!sign) {
    if (mag > (uint32_t)INT_MAX) return S21_CONVERT_ERROR;
    *dst = (int)mag;
  } else {
    if (mag > 2147483648u) return S21_CONVERT_ERROR;
    *dst = (mag == 2147483648u) ? INT_MIN : -(int)mag;
  }
  return 0;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst) return S21_CONVERT_ERROR;
  double two32 = 4294967296.0;            // 2^32
  double two64 = 18446744073709551616.0;  // 2^64
  double mant = (double)(uint32_t)src.bits[0] +
                two32 * (double)(uint32_t)src.bits[1] +
                two64 * (double)(uint32_t)src.bits[2];

  int sc = s21_get_scale(src);
  if (sc > 0) mant /= pow(10.0, sc);
  if (s21_get_sign(src)) mant = -mant;

  *dst = (float)mant;
  return 0;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (!dst) return S21_CONVERT_ERROR;
  if (isnan(src) || isinf(src)) {
    *dst = s21_set_zero();
    return S21_CONVERT_ERROR;
  }
  float axf = fabsf(src);
  if (axf > 0.0f && axf < 1e-28f) {
    *dst = s21_set_zero();
    return S21_CONVERT_ERROR;
  }
  if (axf == 0.0f) {
    *dst = s21_set_zero();
    return 0;
  }

  int sign = (src < 0.0f);
  long double x = (long double)fabs((double)src);

  static const long double P10[29] = {
      1.0L,       10.0L,       100.0L,       1000.0L,       10000.0L, 100000.0L,
      1000000.0L, 10000000.0L, 100000000.0L, 1000000000.0L, 1e10L,    1e11L,
      1e12L,      1e13L,       1e14L,        1e15L,         1e16L,    1e17L,
      1e18L,      1e19L,       1e20L,        1e21L,         1e22L,    1e23L,
      1e24L,      1e25L,       1e26L,        1e27L,         1e28L};

  uint64_t mant = 0;
  int scale = -1;

  for (int k = 0; k <= 28; ++k) {
    long double t = x * P10[k];
    if (t > 1.0e29L) break;
    uint64_t m = (uint64_t)llroundl(t);
    float back = (float)((long double)m / P10[k]);
    if (back == axf) {
      mant = m;
      scale = k;
      break;
    }
  }

  if (scale < 0) {
    int int_digits = (x > 0.0L) ? (int)floorl(log10l(x)) + 1 : 0;
    if (int_digits < 0) int_digits = 0;
    int k = (int_digits < 7) ? (7 - int_digits) : 0;
    if (k > 28) k = 28;
    mant = (uint64_t)llroundl(x * P10[k]);
    if (mant >= 10000000ull && k > 0) {
      mant /= 10ull;
      k--;
    }
    scale = k;
  }

  s21_decimal out = s21_set_zero();
  out.bits[0] = (int)(mant & 0xFFFFFFFFu);
  out.bits[1] = (int)((mant >> 32) & 0xFFFFFFFFu);
  out.bits[2] = 0;

  s21_set_scale(&out, scale);
  s21_set_sign(&out, sign);

  // -0 -> +0
  if (s21_is_zero(out)) s21_set_sign(&out, 0);

  cu_trim_trailing_zeros(&out);

  *dst = out;
  return 0;
}
