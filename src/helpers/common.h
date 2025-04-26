#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>

#include "../lib/s21_decimal.h"

#define MAXINT32 4294967296

void decimalNormalization(s21_decimal *decimal);
void printDecimal(s21_decimal decimal);
int getSign(s21_decimal decimal);
void setSign(s21_decimal *decimal, int sign);
int getPower(s21_decimal decimal);
void setPower(s21_decimal *decimal, int power);
int *getDigitsArray(s21_decimal decimal);
void clear_bits(s21_decimal *value);
s21_decimal getDecimal(int sign, int b2, int b1, int b0, int p);
int decimalChangePower(s21_decimal *decimal, int p);
int addDecimalSameSign(s21_decimal value_1, s21_decimal value_2,
                       s21_decimal *result);
void addDecimalDifferentSign(s21_decimal value_1, s21_decimal value_2,
                             s21_decimal *result);
int decimalToSamePower(s21_decimal *value_1, s21_decimal *value_2);

void s21_init_decimal(s21_decimal *decimal);

#endif
