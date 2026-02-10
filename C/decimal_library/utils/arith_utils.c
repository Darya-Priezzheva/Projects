#include "arith_utils.h"

#include "big_dec_utils.h"
#include "convert_utils.h"
#include "core_utils.h"

void s21_add_big_dec(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result) {
  uint64_t carry = 0;
  for (int i = 0; i < 8; i++) {
    uint64_t sum =
        (uint64_t)value_1.bits[i] + (uint64_t)value_2.bits[i] + carry;
    result->bits[i] = (uint32_t)(sum & ((1ull << 32) - 1));
    carry = sum >> 32;
  }
}

void s21_sub_big_dec(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result) {
  int borrow = 0;
  for (int i = 0; i < 8; i++) {
    int64_t diff = (int64_t)value_1.bits[i] - (int64_t)value_2.bits[i] - borrow;
    result->bits[i] =
        (diff >= 0) ? (unsigned int)diff : (unsigned int)(diff + (1ull << 32));
    borrow = (diff < 0);
  }
}

void s21_mul_big_dec(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result) {
  int count = 0;
  for (int i = 0; i < 256; i++) {
    if (s21_get_big_bit(value_2, i)) {
      s21_shift_left(&value_1, i - count);
      s21_add_big_dec(value_1, *result, result);
      count = i;
    }
  }
}

int s21_div_big_dec(s21_big_decimal value_1, s21_big_decimal value_2,
                    s21_big_decimal *result) {
  int b_1 = 0, b_2 = 0, bit_2 = 0;
  int diff = 0, save_scale = 0;
  s21_big_decimal tmp = {0};

  s21_highest_bit_big_dec(value_1, value_2, &b_1, &b_2);
  bit_2 = b_2;

  for (int i = 0;
       i < 96 && s21_big_dec_not_empty(value_1) && save_scale < 28;) {
    if (i > 0) {
      s21_shift_left(&value_2, 1);
      s21_increase_scale_big_dec(result, 1);
      s21_increase_scale_big_dec(&value_1, 1);
      save_scale++;
    }
    s21_equ_bits_big_dec(&value_1, &value_2);

    b_1 = b_2 = 0;
    s21_highest_bit_big_dec(value_1, value_2, &b_1, &b_2);
    diff = b_2 - bit_2;
    diff = diff < 0 ? 0 : diff;

    while (diff >= 0 && s21_big_dec_not_empty(value_1)) {
      if (s21_greater_big_dec(value_2, value_1)) {
        s21_set_big_bit(&tmp, 0, 0);
      } else {
        s21_sub_big_dec(value_1, value_2, &value_1);
        s21_set_big_bit(&tmp, 0, 1);
      }

      i++;
      diff--;

      if (diff >= 0) {
        s21_shift_right(&value_2, 1);
      }

      s21_shift_left(&tmp, 1);
    }

    if (diff >= 0) {
      s21_shift_left(&tmp, diff + 1);
    }
    s21_shift_right(&tmp, 1);
    s21_add_big_dec(*result, tmp, result);
    s21_zero_big_dec(&tmp);
  }

  return save_scale;
}

int s21_dec_not_empty(s21_decimal value) {
  return value.bits[0] + value.bits[1] + value.bits[2];
}
