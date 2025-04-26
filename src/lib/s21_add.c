#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int er = 0;
  if (getSign(value_1) == getSign(value_2))
    er = addDecimalSameSign(value_1, value_2, result);
  else
    addDecimalDifferentSign(value_1, value_2, result);
  decimalNormalization(result);
  return er;
}
