#include "s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  setSign(&value_2, 1 - getSign(value_2));
  int er = s21_add(value_1, value_2, result);
  decimalNormalization(result);
  return er;
}
