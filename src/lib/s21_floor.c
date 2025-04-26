#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int power = getPower(value);
  int return_value = 0;

  if (!power) {
    *result = value;
  } else {
    clear_bits(result);
    s21_truncate(value, result);
    s21_decimal one = {{1, 0, 0, 0}};
    if (getSign(value)) {
      return_value = s21_sub(*result, one, result);
    }
  }

  return return_value;
}
