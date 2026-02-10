#include <check.h>

#include "../s21_decimal.h"
#include "../utils/core_utils.h"

START_TEST(test_add_1) {
  s21_decimal decimal1 = {{5, 0, 0, 0}};
  s21_decimal decimal2 = {{3, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{8, 0, 0, 0}};

  int code = s21_add(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(result.bits[1], expected.bits[1]);
  ck_assert_uint_eq(result.bits[2], expected.bits[2]);
}
END_TEST

START_TEST(test_add_2) {
  s21_decimal decimal1 = {{5, 0, 0, 0x80000000}};
  s21_decimal decimal2 = {{3, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{8, 0, 0, 0x80000000}};

  int code = s21_add(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_add_3) {
  s21_decimal decimal1 = {{5, 0, 0, 0}};
  s21_decimal decimal2 = {{3, 0, 0, 0}};
  s21_set_sign(&decimal2, 1);
  s21_decimal result;
  s21_decimal expected = {{2, 0, 0, 0}};
  int code = s21_add(decimal1, decimal2, &result);
  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_add_4) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal decimal2 = {{1, 0, 0, 0}};
  s21_decimal result;

  int code = s21_add(decimal1, decimal2, &result);

  ck_assert_int_eq(code, S21_INF_POS);
}
END_TEST

START_TEST(test_add_5) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal decimal2 = {{1, 0, 0, 0x80000000}};
  s21_decimal result;

  int code = s21_add(decimal1, decimal2, &result);

  ck_assert_int_eq(code, S21_INF_NEG);
}
END_TEST

START_TEST(test_add_6) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal result;

  int code = s21_add(decimal1, decimal2, &result);
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(code, S21_OK);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(result.bits[1], expected.bits[1]);
  ck_assert_uint_eq(result.bits[2], expected.bits[2]);
}
END_TEST

START_TEST(test_add_7) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_set_sign(&decimal1, 1);
  s21_decimal decimal2 = {{1, 0, 0, 0}};
  s21_decimal result;
  int code = s21_add(decimal1, decimal2, &result);
  ck_assert_int_eq(code, S21_OK);
  ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_add_8) {
  s21_decimal decimal1 = {{123, 0, 0, 0}};
  s21_set_scale(&decimal1, 28);
  s21_decimal decimal2 = {{1, 0, 0, 0}};
  s21_decimal result;
  int code = s21_add(decimal1, decimal2, &result);
  s21_decimal expected = {{268435579, 1042612833, 542101086, 0}};
  s21_set_scale(&expected, 28);
  ck_assert_int_eq(code, S21_OK);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_1) {
  s21_decimal decimal1 = {{5, 0, 0, 0}};
  s21_decimal decimal2 = {{3, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{2, 0, 0, 0}};

  int code = s21_sub(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_sub_2) {
  s21_decimal decimal1 = {{3, 0, 0, 0}};
  s21_decimal decimal2 = {{5, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{2, 0, 0, 0x80000000}};

  int code = s21_sub(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_sub_3) {
  s21_decimal decimal1 = {{5, 0, 0, 0x80000000}};
  s21_decimal decimal2 = {{3, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{2, 0, 0, 0x80000000}};

  int code = s21_sub(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_sub_4) {
  s21_decimal decimal1 = {{3, 0, 0, 0}};
  s21_decimal decimal2 = {{5, 0, 0, 0}};
  s21_set_sign(&decimal2, 1);
  s21_decimal result;
  s21_decimal expected = {{8, 0, 0, 0}};

  int code = s21_sub(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(s21_get_sign(result), 0);
}
END_TEST

START_TEST(test_sub_5) {
  s21_decimal decimal1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0x00000000}};
  s21_decimal decimal2 = {{1, 0, 0, 0x80000000}};
  s21_decimal result;

  int code = s21_sub(decimal1, decimal2, &result);

  ck_assert_int_eq(code, S21_INF_POS);
}
END_TEST

START_TEST(test_sub_6) {
  s21_decimal decimal1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0x80000000}};
  s21_decimal decimal2 = {{1, 0, 0, 0x00000000}};
  s21_decimal result;

  int code = s21_sub(decimal1, decimal2, &result);

  ck_assert_int_eq(code, S21_INF_NEG);
}
END_TEST

START_TEST(test_sub_7) {
  s21_decimal value_1 = {{13, 0, 0, 0}};
  s21_decimal value_2 = {{286, 0, 0, 0}};
  s21_set_scale(&value_1, 1);
  s21_set_sign(&value_1, 1);
  s21_set_scale(&value_2, 2);
  s21_set_sign(&value_2, 1);
  s21_decimal result = {0};
  s21_decimal expected = {{156, 0, 0, 0}};
  s21_set_scale(&expected, 2);
  int code = s21_sub(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(result.bits[3], expected.bits[3]);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_sub_8) {
  s21_decimal value_1 = {{3, 0, 0, 0}};
  s21_decimal value_2 = {{123, 0, 0, 0}};
  s21_set_scale(&value_2, 28);

  s21_decimal result = {0};

  int code = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_mul_1) {
  s21_decimal decimal1 = {{5, 0, 0, 0}};
  s21_decimal decimal2 = {{3, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{15, 0, 0, 0}};

  int code = s21_mul(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_mul_2) {
  s21_decimal decimal1 = {{5, 0, 0, 0}};
  s21_decimal decimal2 = {{0, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{0, 0, 0, 0}};

  int code = s21_mul(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_mul_3) {
  s21_decimal decimal1 = {{5, 0, 0, 0x80000000}};
  s21_decimal decimal2 = {{3, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{15, 0, 0, 0}};

  int code = s21_mul(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_mul_4) {
  s21_decimal decimal1 = {{5, 0, 0, 0}};
  s21_decimal decimal2 = {{3, 0, 0, 0x80000000}};
  s21_decimal result;
  s21_decimal expected = {{15, 0, 0, 0x80000000}};

  int code = s21_mul(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_mul_5) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal decimal2 = {{2, 0, 0, 0}};
  s21_decimal result;

  int code = s21_mul(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 1);
}
END_TEST
START_TEST(test_mul_6) {
  s21_decimal decimal1 = {{1234, 0, 0, 0}};
  s21_set_scale(&decimal1, 28);
  s21_decimal decimal2 = {{200, 0, 0, 0}};
  s21_decimal result;

  int code = s21_mul(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_div_1) {
  s21_decimal decimal1 = {{10, 0, 0, 0}};
  s21_decimal decimal2 = {{2, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{5, 0, 0, 0}};

  int code = s21_div(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(result.bits[1], expected.bits[1]);
  ck_assert_uint_eq(result.bits[2], expected.bits[2]);
}
END_TEST

START_TEST(test_div_2) {
  s21_decimal decimal1 = {{10, 0, 0, 0}};
  s21_decimal decimal2 = {{3, 0, 0, 0}};
  s21_decimal result;
  int code = s21_div(decimal1, decimal2, &result);
  ck_assert_int_eq(code, 0);
  int scale = s21_get_scale(result);
  ck_assert_int_gt(scale, 0);
  ck_assert_uint_ne(result.bits[0], 3);
}
END_TEST

START_TEST(test_div_3) {
  s21_decimal decimal1 = {{10, 0, 0, 0}};
  s21_decimal decimal2 = {{0, 0, 0, 0}};
  s21_decimal result;

  int code = s21_div(decimal1, decimal2, &result);

  ck_assert_int_eq(code, S21_DIV_BY_ZERO);
}
END_TEST

START_TEST(test_div_4) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal decimal2 = {{2, 0, 0, 0}};
  s21_decimal result;
  int code = s21_div(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_div_5) {
  s21_decimal decimal1 = {{123456, 0, 0, 0}};
  s21_decimal decimal2 = {{12, 0, 0, 0}};
  s21_decimal result;
  s21_set_scale(&decimal1, 3);
  int code = s21_div(decimal1, decimal2, &result);
  s21_decimal expected = {{10288, 0, 0, 0}};
  s21_set_scale(&expected, 3);
  ck_assert_int_eq(code, 0);
  int scale = s21_get_scale(result);
  ck_assert_int_eq(scale, 3);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
}
END_TEST

START_TEST(test_div_6) {
  s21_decimal decimal1 = {{10, 0, 0, 0}};
  s21_decimal decimal2 = {{2, 0, 0, 0}};
  s21_set_sign(&decimal1, 1);
  s21_decimal result;
  s21_decimal expected = {{5, 0, 0, 0}};
  s21_set_sign(&expected, 1);

  int code = s21_div(decimal1, decimal2, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
  ck_assert_uint_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(test_div_7) {
  s21_decimal decimal1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal decimal2 = {{0, 0, 0, 0}};
  s21_decimal result;
  int code = s21_div(decimal1, decimal2, &result);

  ck_assert_int_eq(code, S21_DIV_BY_ZERO);
}
END_TEST

Suite *s21_arithmetic_suite(void) {
  Suite *s = suite_create("s21_arithmetic");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_add_1);
  tcase_add_test(tc, test_add_2);
  tcase_add_test(tc, test_add_3);
  tcase_add_test(tc, test_add_4);
  tcase_add_test(tc, test_add_5);
  tcase_add_test(tc, test_add_6);
  tcase_add_test(tc, test_add_7);
  tcase_add_test(tc, test_add_8);

  tcase_add_test(tc, test_sub_1);
  tcase_add_test(tc, test_sub_2);
  tcase_add_test(tc, test_sub_3);
  tcase_add_test(tc, test_sub_4);
  tcase_add_test(tc, test_sub_5);
  tcase_add_test(tc, test_sub_6);
  tcase_add_test(tc, test_sub_7);
  tcase_add_test(tc, test_sub_8);

  tcase_add_test(tc, test_mul_1);
  tcase_add_test(tc, test_mul_2);
  tcase_add_test(tc, test_mul_3);
  tcase_add_test(tc, test_mul_4);
  tcase_add_test(tc, test_mul_5);
  tcase_add_test(tc, test_mul_6);

  tcase_add_test(tc, test_div_1);
  tcase_add_test(tc, test_div_2);
  tcase_add_test(tc, test_div_3);
  tcase_add_test(tc, test_div_4);
  tcase_add_test(tc, test_div_5);
  tcase_add_test(tc, test_div_6);
  tcase_add_test(tc, test_div_7);

  suite_add_tcase(s, tc);
  return s;
}
