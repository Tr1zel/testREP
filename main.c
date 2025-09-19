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


Входные данные:
знак необязательно.
мантисса не более 30 знаков.
знак порядка обязательно.
Е/е возможно любой вариант.
порядок не более 5 цифр.
*/

int main(void)
{
    size_t len_first_num;
    size_t len_second_num;
    long_number_t first_num;
    long_number_t second_num;
    long_number_t result_num;
    if (parse_input(&first_num, &second_num, &len_first_num, &len_second_num) != 0)
        return ERR_INPUT;
    multiply_long_numbers(&first_num, &second_num, &result_num);
    output_num(result_num);
    return 0;
}
