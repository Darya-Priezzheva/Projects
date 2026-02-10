#include "../../s21_decimal.h"
#include "../../utils/arith_utils.h"
#include "../../utils/big_dec_utils.h"
#include "../../utils/compare_utils.h"
#include "../../utils/core_utils.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = S21_OK;
  *result = s21_set_zero();
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);
  int res_sign = 0;

  if (sign1 && sign2) {
    res_sign = 1;
  }

  if (sign1 != sign2) {
    int sign = sign1;
    s21_set_sign(&value_1, 0);
    s21_set_sign(&value_2, 0);
    if (sign) {
      status = s21_sub(value_2, value_1, result);
    } else {
      status = s21_sub(value_1, value_2, result);
    }
  } else {
    s21_big_decimal a = {0}, b = {0}, res = {0};

    s21_align_scales(&value_1, &value_2);

    s21_dec_to_big(value_1, &a);
    s21_dec_to_big(value_2, &b);

    s21_add_big_dec(a, b, &res);
    int scale = s21_normalization(&res, s21_get_scale(value_1));
    if (scale >= 0) {
      s21_big_to_dec(result, res);
      s21_set_scale(result, scale);
    } else {
      status = S21_INF_POS;
    }
  }

  if (res_sign == 1) {
    s21_set_sign(result, 1);
  }

  if (status == S21_INF_POS && s21_get_sign(*result)) {
    status = S21_INF_NEG;
  }

  if (status) {
    *result = s21_set_zero();
  }

  return status;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = S21_OK;
  *result = s21_set_zero();
  if (s21_get_sign(value_1) && s21_get_sign(value_2)) {
    s21_decimal tmp1 = value_1;
    value_1 = value_2;
    value_2 = tmp1;
    s21_set_bit(&value_1, 127, 0);
    s21_set_bit(&value_2, 127, 0);
  }
  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);
  int res_sign = 0;

  if (sign1 != sign2) {
    if (sign1) res_sign = 1;
    s21_set_sign(&value_1, 0);
    s21_set_sign(&value_2, 0);
    status = s21_add(value_1, value_2, result);
  } else {
    s21_align_scales(&value_1, &value_2);
    s21_big_decimal a = {0}, b = {0}, res = {0};
    s21_dec_to_big(value_1, &a);
    s21_dec_to_big(value_2, &b);

    if (s21_greater_big_dec(b, a)) {
      s21_big_decimal tmp2 = a;
      a = b;
      b = tmp2;
      s21_set_sign(result, 1);
    }
    s21_sub_big_dec(a, b, &res);
    int scale = s21_normalization(&res, s21_get_scale(value_1));
    if (scale >= 0) {
      s21_big_to_dec(result, res);
      s21_set_scale(result, scale);
    } else {
      status = S21_INF_POS;
    }
  }

  res_sign == 1 ? s21_set_sign(result, 1) : 0;

  if (status == S21_INF_POS && s21_get_sign(*result)) status = S21_INF_NEG;

  if (status) *result = s21_set_zero();
  return status;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = S21_OK;
  *result = s21_set_zero();
  s21_big_decimal a = {0}, b = {0}, res = {0};
  s21_dec_to_big(value_1, &a);
  s21_dec_to_big(value_2, &b);

  int sign1 = s21_get_sign(value_1);
  int sign2 = s21_get_sign(value_2);
  int res_sign = sign1 == sign2 ? 0 : 1;
  s21_set_sign(result, res_sign);
  s21_mul_big_dec(a, b, &res);

  int scale = s21_get_scale(value_1) + s21_get_scale(value_2);
  scale = s21_normalization(&res, scale);

  if (scale >= 0) {
    s21_set_scale(result, scale);
    s21_big_to_dec(result, res);
  } else {
    status = S21_INF_POS;
  }

  if (status == S21_INF_POS && s21_get_sign(*result)) {
    status = S21_INF_NEG;
  }

  if (status) {
    *result = s21_set_zero();
  }

  return status;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = S21_OK;
  *result = s21_set_zero();
  if (s21_dec_not_empty(value_2)) {
    s21_big_decimal a = {0}, b = {0}, res = {0};
    s21_dec_to_big(value_1, &a);
    s21_dec_to_big(value_2, &b);

    if (s21_get_sign(value_1) != s21_get_sign(value_2)) s21_set_sign(result, 1);

    int scale = s21_div_big_dec(a, b, &res);
    s21_set_scale(&value_1, s21_get_scale(value_1) + scale);
    scale = s21_get_scale(value_1) - s21_get_scale(value_2);

    if (scale > 0) {
      scale = s21_normalization(&res, scale);
    } else if (scale < 0) {
      s21_increase_scale_big_dec(&res, abs(scale));
      scale = s21_normalization(&res, 0);
    }

    if (scale >= 0) {
      s21_big_to_dec(result, res);
      s21_set_scale(result, scale);
    } else {
      status = S21_INF_POS;
    }
  } else {
    status = S21_DIV_BY_ZERO;
  }
  if (status == 1 && s21_get_sign(*result)) {
    status = S21_INF_NEG;
  }

  if (status) {
    *result = s21_set_zero();
  }

  return status;
}