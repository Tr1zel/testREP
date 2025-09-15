#include <stdio.h>
#include "IO.h"
#include "funcs.h"
/*
Смоделировать операцию умножения целого числа длиной
до 40 десятичных цифр на действительное число в форме
±m.n Е ±K, где суммарная длина мантиссы (m+n) - до 30
значащих цифр, а величина порядка K - до 5 цифр. Результат
выдать в форме ±0.m1 Е ±K1, где m1 - до 40 значащих
цифр, а K1 - до 5 цифр.

макс длина 38 символов строки, если больше то надо округлять
*/

int main(void)
{
    size_t len_first_num;
    size_t len_second_num;
    long_number_t first_num;
    long_number_t second_num;
    long_number_t result_num;
    parse_input(&first_num, &second_num, &len_first_num, &len_second_num);
    multiply_long_numbers(&first_num, &second_num, &result_num);
    // printf("sm = %c, mantissa = %s, se = %c, exp = %d\n", result_num.sign_m, result_num.mantissa, result_num.sign_e, result_num.exponent);
    output_num(result_num);
    return 0;
}
