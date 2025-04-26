#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  unsigned long int r[6] = {};
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      r[i + j] += (unsigned long int)value_1.bits[i] * value_2.bits[j];

  for (int i = 0; i < 5; ++i) {
    r[i + 1] += r[i] / MAXINT32;
    r[i] %= MAXINT32;
  }
  int power = getPower(value_1) + getPower(value_2);
  int sign = getSign(value_1) ^ getSign(value_2);

  unsigned long int d = 0;
  int s = 0;

  for (; power > 28 || (r[5] + r[4] + r[3] > 0 && power > 0); --power) {
    for (int i = 5; i >= 0; --i) {
      d = r[i] + d * ((unsigned long int)1 << 32);
      r[i] = d / 10;
      d = d % 10;
    }
    if (((power <= 29 && r[5] + r[4] + r[3] == 0) || power == 1) &&
        (d > 5 || (d == 5 && (r[0] % 2 == 1 || s > 0)))) {
      d = 1;
      for (int i = 0; i < 6 && d == 1; ++i) {
        d = r[i] + d;
        r[i] = d % MAXINT32;
        d = d / MAXINT32;
      }
    }

    s += d;
    d = 0;
  }
  int er = 0;
  if (r[5] + r[4] + r[3] > 0)
    er = sign + 1;
  else {
    for (int i = 0; i < 3; ++i) result->bits[i] = r[i];
    setSign(result, sign);
    setPower(result, power);
  }
  decimalNormalization(result);
  return er;
}
