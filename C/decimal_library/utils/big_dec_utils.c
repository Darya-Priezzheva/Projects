#include "big_dec_utils.h"

#include "arith_utils.h"

void s21_dec_to_big(s21_decimal value_1, s21_big_decimal *value_2) {
  for (int i = 0; i < 3; i++) {
    value_2->bits[i] = value_1.bits[i];
  }
}

void s21_big_to_dec(s21_decimal *value_1, s21_big_decimal value_2) {
  for (int i = 0; i < 3; i++) {
    value_1->bits[i] = value_2.bits[i];
  }
}

void s21_zero_big_dec(s21_big_decimal *value) {
  for (int i = 0; i < 8; i++) {
    value->bits[i] = 0;
  }
}

void s21_set_big_bit(s21_big_decimal *value, int i, int bit) {
  int show = 1u << (i % 32);
  value->bits[i / 32] =
      (bit == 0) ? (value->bits[i / 32] & ~show) : (value->bits[i / 32] | show);
}

int s21_get_big_bit(s21_big_decimal value, int i) {
  int show = 1u << (i % 32);
  return ((value.bits[i / 32] & show) != 0) ? 1 : 0;
}

void s21_shift_right(s21_big_decimal *value, int number) {
  int bits[7] = {0};
  for (int k = 0; k < number; k++) {
    for (int i = 0; i < 7; i++) {
      bits[i] = s21_get_big_bit(*value, (i + 1) * 32);
    }

    for (int i = 0; i < 7; i++) {
      value->bits[i] >>= 1;
      s21_set_big_bit(value, (i + 1) * 32 - 1, bits[i]);
    }

    value->bits[7] >>= 1;
  }
}

void s21_shift_left(s21_big_decimal *value, int number) {
  int bits[8] = {0};
  for (int i = 0; i < number; i++) {
    for (int j = 0; j < 7; j++) {
      bits[j] = s21_get_big_bit(*value, (j + 1) * 32 - 1);
    }

    for (int i = 7; i > 0; i--) {
      value->bits[i] <<= 1;
      s21_set_big_bit(value, i * 32, bits[i - 1]);
    }

    value->bits[0] <<= 1;
    s21_set_big_bit(value, 0, bits[7]);
  }
}

int s21_normalization(s21_big_decimal *result, int scale) {
  int overflow = (result->bits[3] || result->bits[4] || result->bits[5] ||
                  result->bits[6] || result->bits[7]);
  int rounded = 0;

  while (overflow && scale > 0) {
    if (scale == 1 && result->bits[3]) {
      rounded = 1;
    }

    s21_decreace_scale_big_dec(result, 1);
    scale--;

    overflow = (result->bits[3] || result->bits[4] || result->bits[5] ||
                result->bits[6] || result->bits[7]);
  }

  if (rounded && scale == 0 && result->bits[3] == 0 &&
      s21_get_big_bit(*result, 0)) {
    s21_set_big_bit(result, 0, 0);
  }

  if (overflow) {
    scale = -1;
  }

  return scale;
}

void s21_increase_scale_big_dec(s21_big_decimal *value, int n) {
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};

  for (int i = 0; i < n; i++) {
    s21_mul_big_dec(*value, ten, &tmp);
    *value = tmp;
    s21_zero_big_dec(&tmp);
  }
}

void s21_decreace_scale_big_dec(s21_big_decimal *value, int n) {
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};

  for (int i = 0; i < n; i++) {
    s21_div_big_dec(*value, ten, &tmp);
    *value = tmp;
    s21_zero_big_dec(&tmp);
  }
}

int s21_greater_big_dec(s21_big_decimal value_1, s21_big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !result && !out; i--) {
    if (value_1.bits[i] || value_2.bits[i]) {
      if (value_1.bits[i] > value_2.bits[i]) {
        result = 1;
      }
      if (value_1.bits[i] != value_2.bits[i]) {
        out = 1;
      }
    }
  }
  return result;
}

int s21_is_greater_or_equal_big_decimal(s21_big_decimal value_1,
                                        s21_big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !result && !out; i--) {
    if (value_1.bits[i] != 0 || value_2.bits[i] != 0) {
      if (value_1.bits[i] >= value_2.bits[i]) {
        result = 1;
      }
      out = 1;
    }
  }
  return result;
}

int s21_big_dec_not_empty(s21_big_decimal value) {
  return value.bits[0] + value.bits[1] + value.bits[2] + value.bits[3] +
         value.bits[4] + value.bits[5] + value.bits[6] + value.bits[7];
}

void s21_highest_bit_big_dec(s21_big_decimal t1, s21_big_decimal t2, int *bit_1,
                             int *bit_2) {
  for (int i = 255; i >= 0 && (!(*bit_1) || !(*bit_2)); i--) {
    if (*bit_1 == 0 && s21_get_big_bit(t1, i)) *bit_1 = i;
    if (*bit_2 == 0 && s21_get_big_bit(t2, i)) *bit_2 = i;
  }
}

void s21_equ_bits_big_dec(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  while (s21_is_greater_or_equal_big_decimal(*value_1, *value_2)) {
    s21_shift_left(value_2, 1);
  }

  s21_shift_right(value_2, 1);
}
