#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  setSign(result, getSign(value_1) ^ getSign(value_2));

  unsigned long int r[9] = {};
  unsigned long int res[9] = {};
  for (int i = 6; i < 9; ++i) r[i] = value_1.bits[i - 6];

  if (value_2.bits[0] + value_2.bits[1] + value_2.bits[2] == 0) {
    status = 3;
  } else {
    unsigned long int m[3] = {value_2.bits[0], value_2.bits[1],
                              value_2.bits[2]};

    int left = 2;
    while (m[left] == 0) --left;

    for (int i = 8 - left; i >= 0; --i) {
      unsigned long int s;
      while ((s = ((i + left == 8 ? 0 : r[i + left + 1] * MAXINT32) +
                   r[i + left]) /
                  (m[left] + 1)) > 0) {
        res[i] += s;

        unsigned long int prod[4] = {};
        for (int k = 0; k < 3; ++k) {
          unsigned long int v = s * value_2.bits[k];
          prod[k] += v % MAXINT32;
          prod[k + 1] += v / MAXINT32;
        }
        unsigned long int d = 0;
        for (int j = 0; j <= left; ++j) {
          if (r[i + j] >= d + prod[j]) {
            r[i + j] = r[i + j] - prod[j] - d;
            d = 0;
          } else {
            r[i + j] = MAXINT32 + r[i + j] - prod[j] - d;
            d = 1;
          }
        }
        if (i + left + 1 <= 8) r[i + left + 1] -= d + prod[left + 1];
      }
    }

    int power = getPower(value_1) - getPower(value_2);
    unsigned long int d = 0;
    while (power < 28 && res[8] < (MAXINT32 - 10) / 100) {
      for (int i = 0; i < 9; ++i) {
        res[i] = res[i] * 10 + d;
        d = res[i] / MAXINT32;
        res[i] = res[i] % MAXINT32;
      }
      ++power;
    }

    if (power < 0) {
      status = getSign(*result) + 1;
    } else {
      d = (res[5] * 10) / MAXINT32;
      if (((d > 5 || d == 5) &&
           (res[6] % 2 == 1 || 10 * res[5] - d * MAXINT32 + res[4] + res[3] +
                                       res[2] + res[1] + res[0] >
                                   0))) {
        unsigned long int p = 1;
        for (int i = 6; i < 9 && p == 1; ++i) {
          p = res[i] + p;
          res[i] = p % MAXINT32;
          p = p / MAXINT32;
        }
      }

      for (int i = 0; i < 3; ++i) result->bits[i] = res[6 + i];
      setPower(result, power);
      decimalNormalization(result);
    }
  }

  return status;
}
