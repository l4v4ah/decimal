#include "../helpers/common.h"
#include "s21_decimal.h"

int s21_is_equal(s21_decimal d1, s21_decimal d2) {
  decimalNormalization(&d1);
  decimalNormalization(&d2);
  int f = 1;
  for (int i = 0; i < 4; ++i)
    if (d1.bits[i] != d2.bits[i]) f = 0;
  return f;
}
