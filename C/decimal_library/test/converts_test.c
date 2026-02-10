#include <check.h>

#include "../s21_decimal.h"
#include "../utils/core_utils.h"

START_TEST(test_int_to_decimal_1) {
  int src = 1234;
  s21_decimal dst;
  int code = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(src, dst.bits[0]);
}
END_TEST

START_TEST(test_int_to_decimal_2) {
  int src = INT32_MAX;
  s21_decimal dst;
  int code = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(src, dst.bits[0]);
}
END_TEST

START_TEST(test_int_to_decimal_3) {
  int src = -1234;
  s21_decimal dst;
  int code = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(-src, dst.bits[0]);
}
END_TEST

START_TEST(test_int_to_decimal_4) {
  int src = INT32_MIN;
  s21_decimal dst;
  int code = s21_from_int_to_decimal(src, &dst);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(src, dst.bits[0]);
}
END_TEST

START_TEST(test_decimal_to_int_1) {
  s21_decimal src = {{100, 0, 0, 0}};
  int dst;
  int check = 100;
  int code = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(dst, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_decimal_to_int_2) {
  s21_decimal src = {{100, 0, 0, 0}};
  s21_set_sign(&src, 1);
  int dst;
  int check = -100;
  int code = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(dst, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_decimal_to_int_3) {
  s21_decimal src = {{21212121, 0, 0, 0}};
  s21_set_scale(&src, 2);
  int dst;
  int check = 212121;
  int code = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(dst, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_decimal_to_int_4) {
  s21_decimal src = {{12345678, 0, 0, 0}};
  s21_set_scale(&src, 3);
  s21_set_sign(&src, 1);
  int dst;
  int check = -12345;
  int code = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(dst, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_decimal_to_int_5) {
  s21_decimal src = {{INT_MAX, 0, 0, 0}};
  int dst;
  int check = INT_MAX;
  int code = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(dst, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_decimal_to_int_6) {
  s21_decimal src = {{INT_MAX, INT_MAX, 0, 0}};
  int dst;
  int code = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_decimal_to_int_7) {
  s21_decimal src = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  int dst;
  int code = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_decimal_to_int_8) {
  s21_decimal src = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
  s21_set_scale(&src, 28);
  int dst;
  int code = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_float_to_decimal_1) {
  float src = 123.45f;
  s21_decimal dst;
  int code = s21_from_float_to_decimal(src, &dst);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(12345, dst.bits[0]);
  ck_assert_int_eq(2, s21_get_scale(dst));
}
END_TEST

START_TEST(test_float_to_decimal_2) {
  float src = -2147483648.56f;
  s21_decimal dst;
  int code = s21_from_float_to_decimal(src, &dst);
  float check;
  s21_from_decimal_to_float(dst, &check);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq(src, check);
}
END_TEST

START_TEST(test_float_to_decimal_3) {
  float src = 0.000123f;
  s21_decimal dst;
  int code = s21_from_float_to_decimal(src, &dst);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(123, dst.bits[0]);
  ck_assert_int_eq(6, s21_get_scale(dst));
  ck_assert_int_eq(0, s21_get_sign(dst));
}
END_TEST

START_TEST(test_float_to_decimal_4) {
  float src = 1000.0f;
  s21_decimal dst;
  int code = s21_from_float_to_decimal(src, &dst);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(1000, dst.bits[0]);
  ck_assert_int_eq(0, s21_get_scale(dst));
  ck_assert_int_eq(0, s21_get_sign(dst));
}
END_TEST

START_TEST(test_float_to_decimal_5) {
  float src = NAN;
  s21_decimal dst;
  int code = s21_from_float_to_decimal(src, &dst);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_float_to_decimal_6) {
  float src = INFINITY;
  s21_decimal dst;
  int code = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_float_to_decimal_7) {
  float src = 1e-30f;
  s21_decimal dst;
  int code = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_float_to_decimal_8) {
  float src = 0.000123f;
  s21_decimal dst;
  int code = s21_from_float_to_decimal(src, &dst);
  float check;
  s21_from_decimal_to_float(dst, &check);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq(src, check);
  ck_assert_int_eq(123, dst.bits[0]);
  ck_assert_int_eq(6, s21_get_scale(dst));
  ck_assert_int_eq(0, s21_get_sign(dst));
}
END_TEST

START_TEST(test_float_to_decimal_9) {
  float src = 1234.67f;
  s21_decimal dst;
  int code = s21_from_float_to_decimal(src, &dst);
  float check;
  s21_from_decimal_to_float(dst, &check);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq(src, check);
  // ck_assert_int_eq(123, dst.bits[0]);
  // ck_assert_int_eq(6, s21_get_scale(dst));
  // ck_assert_int_eq(0, s21_get_sign(dst));
}
END_TEST

START_TEST(test_float_to_decimal_10) {
  float src = 0.0009876f;
  s21_decimal dst;

  int code = s21_from_float_to_decimal(src, &dst);
  float check;
  s21_from_decimal_to_float(dst, &check);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq(src, check);
}
END_TEST

START_TEST(test_float_to_decimal_11) {
  float src = -1234.7483f;
  s21_decimal dst;

  int code = s21_from_float_to_decimal(src, &dst);
  float check;
  s21_from_decimal_to_float(dst, &check);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq(src, check);
  // ck_assert_int_eq(1234748, dst.bits[0]);
}
END_TEST

START_TEST(test_float_to_decimal_12) {
  float src = 1627489364.536286f;
  s21_decimal dst;

  int code = s21_from_float_to_decimal(src, &dst);
  float check;
  s21_from_decimal_to_float(dst, &check);
  ck_assert_int_eq(code, 0);
  ck_assert_float_eq(src, check);
}
END_TEST

START_TEST(test_float_to_decimal_13) {
  float src = 1e-28f;
  s21_decimal dst;
  int code = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_decimal_to_float_1) {
  s21_decimal val = {{123456789, 0, 0, 0}};
  s21_set_sign(&val, 1);
  s21_set_scale(&val, 5);
  float res = -1234.56789;
  float dst;
  s21_from_decimal_to_float(val, &dst);
  ck_assert_float_eq(dst, res);
}
END_TEST

START_TEST(test_decimal_to_float_2) {
  s21_decimal src = {{1812, 0, 0, 0}};
  s21_set_sign(&src, 1);
  float dst = 0;
  float check = -1812;
  int code = s21_from_decimal_to_float(src, &dst);
  ck_assert_float_eq(dst, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_decimal_to_float_3) {
  s21_decimal src = {{123, 0, 0, 0}};
  s21_set_scale(&src, 1);
  float dst = 0;
  float check = 12.3;
  int code = s21_from_decimal_to_float(src, &dst);
  ck_assert_float_eq(dst, check);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_decimal_to_float_4) {
  s21_decimal src = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
  s21_set_scale(&src, 28);
  float check = 7.922816;
  float dst;
  int code = s21_from_decimal_to_float(src, &dst);

  float eps = 1e-6f;
  ck_assert_int_eq(code, 0);
  ck_assert(fabsf(dst - check) < eps);
}
END_TEST

Suite *s21_converts_suite(void) {
  Suite *s = suite_create("s21_converts");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_int_to_decimal_1);
  tcase_add_test(tc, test_int_to_decimal_2);
  tcase_add_test(tc, test_int_to_decimal_3);
  tcase_add_test(tc, test_int_to_decimal_4);
  tcase_add_test(tc, test_decimal_to_int_1);
  tcase_add_test(tc, test_decimal_to_int_2);
  tcase_add_test(tc, test_decimal_to_int_3);
  tcase_add_test(tc, test_decimal_to_int_4);
  tcase_add_test(tc, test_decimal_to_int_5);
  tcase_add_test(tc, test_decimal_to_int_6);
  tcase_add_test(tc, test_decimal_to_int_7);
  tcase_add_test(tc, test_decimal_to_int_8);
  tcase_add_test(tc, test_float_to_decimal_1);
  tcase_add_test(tc, test_float_to_decimal_2);
  tcase_add_test(tc, test_float_to_decimal_3);
  tcase_add_test(tc, test_float_to_decimal_4);
  tcase_add_test(tc, test_float_to_decimal_5);
  tcase_add_test(tc, test_float_to_decimal_6);
  tcase_add_test(tc, test_float_to_decimal_7);
  tcase_add_test(tc, test_float_to_decimal_8);
  tcase_add_test(tc, test_float_to_decimal_9);
  tcase_add_test(tc, test_float_to_decimal_10);
  tcase_add_test(tc, test_float_to_decimal_11);
  tcase_add_test(tc, test_float_to_decimal_12);
  tcase_add_test(tc, test_float_to_decimal_13);
  tcase_add_test(tc, test_decimal_to_float_1);
  tcase_add_test(tc, test_decimal_to_float_2);
  tcase_add_test(tc, test_decimal_to_float_3);
  tcase_add_test(tc, test_decimal_to_float_4);
  suite_add_tcase(s, tc);
  return s;
}