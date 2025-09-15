#ifndef IO_H
#define IO_H
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define ERR_FLOAT 3
typedef struct
{
    char sign_m;       // Знак мантиссы: '+' или '-'
    char mantissa[31]; // Массив для 30 знаков мантиссы + 1 для '\0'. Хранится как строка цифр.
    char sign_e;       // Знак порядка: '+' или '-'
    int exponent;      // Само значение порядка (от 0 до 99999)
} long_number_t;

int parse_input(long_number_t *first_num, long_number_t *second_num, size_t *len_first_num, size_t *len_second_num);
void output_num(long_number_t result_num);

#endif
