CC = gcc -std=c11
FLAGS = -Wall -Wextra -Werror
C_SOURCES = $(wildcard lib/*.c) $(wildcard helpers/*.c)
# Ниже настройки сборки файлов для разных систем
UNAME_S := $(shell uname -s)
OS := $(shell cat /etc/os-release)
ifeq ($(UNAME_S),Darwin)# Команды для MacOS
    LCHECK=-lcheck #-mmacosx-version-min=13.0
else ifeq ($(findstring Ubuntu, $(OS)), Ubuntu)
    LCHECK=-lcheck -lsubunit -lm
else ifeq ($(findstring Manjaro, $(OS)), Manjaro)
	LCHECK=-lcheck -lm
else
	LCHECK=-lcheck -lm
endif


all: s21_decimal.a
	$(CC) test.c s21_decimal.a -o test $(LCHECK)

test: s21_decimal.a
	$(CC) test.c s21_decimal.a -o test $(LCHECK)
gcov_report: s21_decimal.a
	$(CC) test.c $(C_SOURCES) -o test $(LCHECK) -fprofile-arcs -ftest-coverage
	./test
	lcov --capture --directory . -o coverage.info
	genhtml coverage.info -o coverage_html

s21_decimal.a:
	$(CC) $(FLAGS) -c $(C_SOURCES)
	ar rc s21_decimal.a *.o
	
clean:
	rm -rf *.o *.a *.gcda *.gcno *.info coverage_html
