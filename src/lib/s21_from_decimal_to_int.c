#include "../helpers/common.h"
#include "s21_decimal.h"
void divide_by_10(s21_decimal *src);

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;
  int sign = getSign(src);
  int power = getPower(src);
  // отбрасываем дробную часть
  while (power > 0) {
    divide_by_10(&src);
    power--;
  }
  if (src.bits[1] != 0 || src.bits[2] != 0)
    error++;
  else {
    *dst = (int)src.bits[0];
    if (sign) *dst *= -1;
  }
  return error;
}

void divide_by_10(s21_decimal *src) {
  long long unsigned int remainder = 0;
  for (int i = 2; i >= 0; i--) {
    long long unsigned current = (remainder << 32) | src->bits[i];
    src->bits[i] = current / 10;
    remainder = current % 10;
  }
}
