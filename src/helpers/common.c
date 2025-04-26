#include "common.h"

void decimalNormalization(s21_decimal *decimal) {
  if (decimal->bits[0] == 0 && decimal->bits[1] == 0 && decimal->bits[2] == 0) {
    decimal->bits[3] = 0;
    return;
  }
  int power = getPower(*decimal);
  while (
      (6 * ((long int)decimal->bits[1] + decimal->bits[2]) + decimal->bits[0]) %
              10 ==
          0 &&
      power > 0) {
    unsigned long int p = 0;
    for (int i = 2; i >= 0; --i) {
      unsigned long int v = decimal->bits[i] + p * ((long int)1 << 32);
      decimal->bits[i] = v / 10;
      p = v % 10;
    }
    --power;
  }
  setPower(decimal, power);
}

int getSign(s21_decimal decimal) { return decimal.bits[3] >> 31; }

void setSign(s21_decimal *decimal, int sign) {
  if (sign == 0)
    decimal->bits[3] &= 0x7FFFFFFF;
  else
    decimal->bits[3] |= 0x80000000;
}

int getPower(s21_decimal decimal) {
  return (decimal.bits[3] & 0x00FF0000) >> 16;
}

void setPower(s21_decimal *decimal, int power) {
  int powerPosition = power << 16;
  unsigned int sign = getSign(*decimal);
  decimal->bits[3] = (sign << 31) + powerPosition;
}

int *getDigitsArray(s21_decimal decimal) {
  int *a = calloc(100, sizeof(int));
  for (int j = 0; j < 100; ++j) {
    a[j] =
        (6 * ((long int)decimal.bits[1] + decimal.bits[2]) + decimal.bits[0]) %
        10;
    unsigned long int p = 0;
    for (int i = 2; i >= 0; --i) {
      unsigned long int v = decimal.bits[i] + p * ((long int)1 << 32);
      decimal.bits[i] = v / 10;
      p = v % 10;
    }
  }
  return a;
}

void clear_bits(s21_decimal *value) {
  memset(value->bits, 0, sizeof(value->bits));
}

s21_decimal getDecimal(int sign, int b2, int b1, int b0, int p) {
  s21_decimal res;
  res.bits[2] = b2;
  res.bits[1] = b1;
  res.bits[0] = b0;
  res.bits[3] = (p << 16) + (sign << 31);
  return res;
}

int decimalChangePower(s21_decimal *decimal, int p) {
  int power = getPower(*decimal);

  unsigned long int d = 0;
  for (; power < p && d == 0; ++power) {
    d = 0;
    for (int j = 0; j < 3; ++j) {
      d = decimal->bits[j] * (unsigned long int)10 + d;
      decimal->bits[j] = d % MAXINT32;
      d = d / MAXINT32;
    }
  }
  if (d > 0) p = power - 1;

  int s = 0;
  for (; power > p; --power) {
    for (int i = 2; i >= 0; --i) {
      d = decimal->bits[i] + d * ((unsigned long int)1 << 32);
      decimal->bits[i] = d / 10;
      d = d % 10;
    }
    if ((power == p + 1) &&
        (d > 5 || (d == 5 && (decimal->bits[0] % 2 == 1 || s > 0)))) {
      d = 1;
      for (int i = 0; i < 3 && d == 1; ++i) {
        d = decimal->bits[i] + d;
        decimal->bits[i] = d % MAXINT32;
        d = d / MAXINT32;
      }
    }
    s += d;
    d = 0;
  }
  setPower(decimal, power);
  return power;
}

int decimalToSamePower(s21_decimal *value_1, s21_decimal *value_2) {
  decimalNormalization(value_1);
  decimalNormalization(value_2);
  int p1 = getPower(*value_1);
  int p2 = getPower(*value_2);
  int mp = p1;
  if (p1 > p2) {
    mp = decimalChangePower(value_2, p1);
    decimalChangePower(value_1, mp);
  } else if (p2 > p1) {
    mp = decimalChangePower(value_1, p2);
    decimalChangePower(value_2, mp);
  }
  return mp;
}

int addDecimalSameSign(s21_decimal value_1, s21_decimal value_2,
                       s21_decimal *result) {
  int s1 = getSign(value_1);
  int mp = decimalToSamePower(&value_1, &value_2);

  setSign(result, s1);
  int er = 0;

  unsigned long int d = 0;
  for (int i = 0; i < 3; ++i) {
    d = d + value_1.bits[i] + value_2.bits[i];
    result->bits[i] = d % MAXINT32;
    d = d / MAXINT32;
  }
  if (d == 1 && mp == 0)
    er = s1 + 1;
  else if (d == 1) {
    --mp;
    decimalChangePower(&value_1, mp);
    decimalChangePower(&value_2, mp);
    d = 0;
    for (int i = 0; i < 3; ++i) {
      d = d + value_1.bits[i] + value_2.bits[i];
      result->bits[i] = d % MAXINT32;
      d = d / MAXINT32;
    }
  }
  setPower(result, mp);
  return er;
}

void addDecimalDifferentSign(s21_decimal value_1, s21_decimal value_2,
                             s21_decimal *result) {
  int s1 = getSign(value_1), s2 = getSign(value_2);
  int mp = decimalToSamePower(&value_1, &value_2);

  setSign(result, s1);
  setSign(&value_1, 0);
  setSign(&value_2, 0);
  s21_decimal v1 = value_1, v2 = value_2;

  if (s21_is_less(value_1, value_2)) {
    v1 = value_2;
    v2 = value_1;
    setSign(result, s2);
  }
  unsigned long int d = 0;
  for (int i = 0; i < 3; ++i) {
    if (v1.bits[i] >= d + v2.bits[i]) {
      result->bits[i] = v1.bits[i] - v2.bits[i] - d;
      d = 0;
    } else {
      result->bits[i] = MAXINT32 + v1.bits[i] - v2.bits[i] - d;
      d = 1;
    }
  }

  setPower(result, mp);
}

void s21_init_decimal(s21_decimal *decimal) {
  for (int i = 0; i < 4; i++) {
    decimal->bits[i] = 0;
  }
}
