#include <check.h>
#include <stdlib.h>

Suite *s21_arithmetic_suite();
Suite *s21_other_suite(void);
Suite *s21_compare_suite(void);
Suite *s21_converts_suite(void);

int main(void) {
  int failed = 0;
  Suite *s;
  SRunner *sr;

  s = s21_arithmetic_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  s = s21_other_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  s = s21_compare_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  s = s21_converts_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}