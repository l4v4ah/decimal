#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  // cleaning data
  memset(dst, 0, sizeof(*dst));
  // set int without sign
  dst->bits[0] = abs(src);
  // sign
  if (src < 0) dst->bits[3] |= (1U << 31);
  return 0;
}