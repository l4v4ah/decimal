#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  clear_bits(result);
  int return_value = 0;
  int sign = getSign(value);
  setSign(&value, 0);

  s21_truncate(value, result);
  s21_decimal diff = {{0}};
  s21_sub(value, *result, &diff);

  s21_decimal zero_five = {{5, 0, 0, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  setPower(&zero_five, 1);

  if (s21_is_greater_or_equal(diff, zero_five)) {
    return_value = s21_add(*result, one, result);
  }

  if (sign) setSign(result, 1);
  return return_value;
}