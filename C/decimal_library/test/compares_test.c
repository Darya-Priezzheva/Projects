#include <check.h>

#include "../s21_decimal.h"
#include "../utils/compare_utils.h"
#include "../utils/core_utils.h"

START_TEST(test_equal_1) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  int code = s21_is_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_equal_2) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_set_sign(&a, 1);
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_sign(&b, 1);
  int code = s21_is_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_equal_3) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_sign(&a, 1);
  s21_set_scale(&a, 2);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_sign(&b, 1);
  s21_set_scale(&b, 2);
  int code = s21_is_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_equal_4) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{6, 0, 0, 0}};
  int code = s21_is_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_equal_5) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_set_scale(&b, 1);
  int code = s21_is_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_equal_6) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_sign(&a, 1);
  s21_set_scale(&a, 2);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_scale(&b, 1);
  int code = s21_is_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_not_equal_1) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_decimal b = {{12345, 0, 0, 0}};
  int code = s21_is_not_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_not_equal_2) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_sign(&a, 1);
  s21_decimal b = {{12345, 0, 0, 0}};
  int code = s21_is_not_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_not_equal_3) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_scale(&a, 2);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_scale(&b, 1);
  int code = s21_is_not_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_not_equal_4) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_decimal b = {{51234, 0, 0, 0}};
  int code = s21_is_not_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_not_equal_5) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_sign(&a, 1);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_sign(&b, 1);
  int code = s21_is_not_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_not_equal_6) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_scale(&a, 2);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_scale(&b, 2);
  int code = s21_is_not_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_less_1) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  int code = s21_is_less(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_less_2) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_set_sign(&a, 1);
  s21_decimal b = {{2, 0, 0, 0}};
  int code = s21_is_less(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_less_3) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_scale(&a, 2);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_scale(&b, 1);
  int code = s21_is_less(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_less_4) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  int code = s21_is_less(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_less_5) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  s21_set_sign(&b, 1);
  int code = s21_is_less(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_less_6) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_scale(&a, 1);  // 5123.4
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_scale(&b, 2);  // 512.34
  int code = s21_is_less(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_less_eq_1) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  int code = s21_is_less_or_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_less_eq_2) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_set_sign(&a, 1);
  s21_decimal b = {{2, 0, 0, 0}};
  int code = s21_is_less_or_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_less_eq_3) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_scale(&a, 2);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_scale(&b, 3);
  int code = s21_is_less_or_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_less_eq_4) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  int code = s21_is_less_or_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_less_eq_5) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  s21_set_sign(&b, 1);
  int code = s21_is_less_or_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_less_eq_6) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_scale(&a, 3);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_scale(&b, 2);
  int code = s21_is_less_or_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_greater_1) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  int code = s21_is_greater(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_greater_2) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_set_sign(&a, 1);
  s21_decimal b = {{2, 0, 0, 0}};
  int code = s21_is_greater(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_greater_3) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_scale(&a, 3);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_scale(&b, 2);
  int code = s21_is_greater(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_greater_4) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  int code = s21_is_greater(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_greater_5) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_set_sign(&a, 1);
  s21_decimal b = {{2, 0, 0, 0}};
  int code = s21_is_greater(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_greater_6) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_scale(&a, 2);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_scale(&b, 3);
  int code = s21_is_greater(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_greater_eq_1) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  int code = s21_is_greater_or_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_greater_eq_2) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  int code = s21_is_greater_or_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_is_greater_eq_3) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_scale(&a, 2);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_scale(&b, 1);
  int code = s21_is_greater_or_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_greater_eq_4) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  int code = s21_is_greater_or_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_greater_eq_5) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_set_sign(&a, 1);
  s21_decimal b = {{2, 0, 0, 0}};
  int code = s21_is_greater_or_equal(a, b);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(test_is_greater_eq_6) {
  s21_decimal a = {{51234, 0, 0, 0}};
  s21_set_scale(&a, 1);
  s21_decimal b = {{51234, 0, 0, 0}};
  s21_set_scale(&b, 2);
  int code = s21_is_greater_or_equal(a, b);
  ck_assert_int_eq(code, 1);
}
END_TEST

Suite *s21_compare_suite(void) {
  Suite *s = suite_create("s21_compare");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_equal_1);
  tcase_add_test(tc, test_equal_2);
  tcase_add_test(tc, test_equal_3);
  tcase_add_test(tc, test_equal_4);
  tcase_add_test(tc, test_equal_5);
  tcase_add_test(tc, test_equal_6);

  tcase_add_test(tc, test_is_not_equal_1);
  tcase_add_test(tc, test_is_not_equal_2);
  tcase_add_test(tc, test_is_not_equal_3);
  tcase_add_test(tc, test_is_not_equal_4);
  tcase_add_test(tc, test_is_not_equal_5);
  tcase_add_test(tc, test_is_not_equal_6);

  tcase_add_test(tc, test_is_less_1);
  tcase_add_test(tc, test_is_less_2);
  tcase_add_test(tc, test_is_less_3);
  tcase_add_test(tc, test_is_less_4);
  tcase_add_test(tc, test_is_less_5);
  tcase_add_test(tc, test_is_less_6);

  tcase_add_test(tc, test_is_less_eq_1);
  tcase_add_test(tc, test_is_less_eq_2);
  tcase_add_test(tc, test_is_less_eq_3);
  tcase_add_test(tc, test_is_less_eq_4);
  tcase_add_test(tc, test_is_less_eq_5);
  tcase_add_test(tc, test_is_less_eq_6);

  tcase_add_test(tc, test_is_greater_1);
  tcase_add_test(tc, test_is_greater_2);
  tcase_add_test(tc, test_is_greater_3);
  tcase_add_test(tc, test_is_greater_4);
  tcase_add_test(tc, test_is_greater_5);
  tcase_add_test(tc, test_is_greater_6);

  tcase_add_test(tc, test_is_greater_eq_1);
  tcase_add_test(tc, test_is_greater_eq_2);
  tcase_add_test(tc, test_is_greater_eq_3);
  tcase_add_test(tc, test_is_greater_eq_4);
  tcase_add_test(tc, test_is_greater_eq_5);
  tcase_add_test(tc, test_is_greater_eq_6);

  suite_add_tcase(s, tc);
  return s;
}
