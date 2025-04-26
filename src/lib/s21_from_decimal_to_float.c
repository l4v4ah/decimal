#include <float.h>
#include <math.h>

#include "../helpers/common.h"
#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error = 0;
  long double result = 0.0;
  int power = getPower(src);
  int sign = getSign(src) ? -1 : 1;
  for (int i = 0; i < 3; i++) {
    float temp = (float)src.bits[i] / pow(10, power);
    result += temp * powf(2.0f, i * 32);
  }
  if (!error) *dst = result * sign;
  return error;
}