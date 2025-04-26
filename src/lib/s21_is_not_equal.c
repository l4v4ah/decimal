#include "s21_decimal.h"

int s21_is_not_equal(s21_decimal d1, s21_decimal d2) {
  return !s21_is_equal(d1, d2);
}
