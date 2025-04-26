#include <stdlib.h>

#include "../helpers/common.h"
#include "s21_decimal.h"

int s21_is_less(s21_decimal d1, s21_decimal d2) {
  decimalNormalization(&d1);
  decimalNormalization(&d2);
  int sign1 = getSign(d1);
  int sign2 = getSign(d2);
  int result = 0;

  if (sign1 + sign2 == 1) {
    result = sign1 > sign2;
  } else {
    int *a1 = getDigitsArray(d1), power1 = getPower(d1);
    int *a2 = getDigitsArray(d2), power2 = getPower(d2);

    int d = power1 - power2;
    int i = 99 + d;
    if (i < 99) i = 99;

    for (; i >= 0 || i - d >= 0; --i) {
      int v1 = 0, v2 = 0;
      if (i <= 99 && i >= 0) v1 = a1[i];
      if (i - d <= 99 && i - d >= 0) v2 = a2[i - d];
      if (v1 < v2) {
        result = 1 - sign1;
        break;
      }
      if (v1 > v2) {
        result = sign1;
        break;
      }
    }
    free(a1);
    free(a2);
  }

  return result;
}
