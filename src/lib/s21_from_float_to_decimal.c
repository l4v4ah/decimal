#include <math.h>

#include "../helpers/common.h"
#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  // предварительная проверка аргументов по принципам структурного
  // программирования
  if (fabsf(src) > 79228162514264337593543950335.0L || isinf(src)) return 1;
  memset(dst, 0, sizeof(*dst));
  if (0 < fabsf(src) && fabsf(src) < 1e-28) return 1;
  if (fabsf(src) < 1e-28) return 0;

  // sign
  if (src < 0) {
    dst->bits[3] |= (1U << 31);
    src = fabsf(src);
  }
  long double x = src;  // * pow(10, 7);
  int p = 0;
  while (x < 1000000) {
    x = x * 10;
    p += 1;
  }
  while (x >= 10000000) {
    x = x / 10;
    p -= 1;
  }
  long unsigned int n = (long unsigned int)round(x);
  dst->bits[0] = (unsigned int)n;
  dst->bits[1] = (unsigned int)(n >> 32);
  // dst->bits[3] |= 7 << 16;
  if (p >= 0)
    dst->bits[3] |= p << 16;
  else {
    s21_decimal d2 = getDecimal(0, 0, 0, 10, 0);
    while (p < 0) {
      s21_mul(*dst, d2, dst);
      ++p;
    }
  }
  decimalNormalization(dst);
  return 0;
}