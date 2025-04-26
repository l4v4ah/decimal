#include "../helpers/common.h"
#include "s21_decimal.h"

void s21_divide_by_integer(s21_decimal value, int integer,
                           s21_decimal *result) {
  unsigned long long buf = 0ull;

  for (int i = 2; i >= 0; i--) {
    unsigned long long div_buf;
    unsigned mod;
    mod = (buf + value.bits[i]) % integer;
    div_buf = (buf + value.bits[i]) / integer;
    result->bits[i] = (unsigned)div_buf;
    buf = mod;
    buf = buf << 32;
  }
}

void s21_divide_by_power_of_10(s21_decimal value, int power,
                               s21_decimal *result) {
  s21_decimal tmp_result = value;
  for (int i = 0; i < power; i++) {
    s21_divide_by_integer(tmp_result, 10, result);
    tmp_result = *result;
  }
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  *result = value;
  int scale = getPower(value);
  result->bits[3] &= 0x80000000;
  s21_divide_by_power_of_10(*result, scale, result);

  return 0;
}