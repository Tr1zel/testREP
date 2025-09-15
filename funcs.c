#include "funcs.h"
#include "IO.h"

#define BASE 10000 // Основание системы счисления (4 цифры в блоке)
#define MAX_BLOCKS 10

// Функция нормализации мантиссы к формату 0.xxxxx
void normalize_mantissa(char *mantissa, int *exponent, char *sign_e)
{
    // Убираем ведущие нули
    int start = 0;
    while (mantissa[start] == '0' && mantissa[start + 1] != '\0')
        start++;

    if (start > 0)
    {
        memmove(mantissa, mantissa + start, strlen(mantissa) - start + 1);
        // Корректируем порядок
        if (*sign_e == '+')
            *exponent -= start;
        else
            *exponent += start;
    }
}

// Вспомогательная функция для преобразования строки в массив блоков
void string_to_blocks(const char *str, int blocks[], int *num_blocks)
{
    int len = strlen(str);
    *num_blocks = 0;

    // Обрабатываем строку с конца (младшие разряды)
    for (int i = len - 1; i >= 0; i -= 4)
    {
        int block = 0;
        int power = 1;

        // Формируем один блок из 4 цифр
        for (int j = 0; j < 4; j++)
        {
            int pos = i - j;
            if (pos >= 0 && isdigit(str[pos]))
            {
                block += (str[pos] - '0') * power;
                power *= 10;
            }
        }

        blocks[(*num_blocks)++] = block;
    }
}

// Функция умножения двух массивов блоков
void multiply_blocks(const int a[], int a_len, const int b[], int b_len, int result[], int *result_len)
{
    // Инициализируем результат нулями
    *result_len = a_len + b_len;
    for (int i = 0; i < *result_len; i++)
        result[i] = 0;
    // Умножение в столбик
    for (int i = 0; i < a_len; i++)
    {
        int carry = 0;
        for (int j = 0; j < b_len; j++)
        {
            int temp = a[i] * b[j] + result[i + j] + carry;
            result[i + j] = temp % BASE;
            carry = temp / BASE;
        }
        result[i + b_len] = carry;
    }

    // Убираем ведущие нули
    while (*result_len > 1 && result[*result_len - 1] == 0)
        (*result_len)--;
}

// Функция преобразования блоков обратно в строку
void blocks_to_string(const int blocks[], int num_blocks, char *result)
{
    int pos = 0;
    // Старший блок без ведущих нулей
    pos += sprintf(result + pos, "%d", blocks[num_blocks - 1]);
    // Остальные блоки с ведущими нулями
    for (int i = num_blocks - 2; i >= 0; i--)
        pos += sprintf(result + pos, "%04d", blocks[i]); // 04d гарантирует 4 цифры
    result[pos] = '\0';
}

// Основная функция умножения
int multiply_long_numbers(const long_number_t *a, const long_number_t *b, long_number_t *result)
{
    // 1. Определяем знак результата
    result->sign_m = (a->sign_m == b->sign_m) ? '+' : '-';

    // 2. Обрабатываем нули
    if (strcmp(a->mantissa, "0") == 0 || strcmp(b->mantissa, "0") == 0)
    {
        strcpy(result->mantissa, "0");
        result->exponent = 0;
        result->sign_e = '+';
        return 0;
    }

    // 3. Преобразуем мантиссы в блоки
    int a_blocks[MAX_BLOCKS], b_blocks[MAX_BLOCKS], res_blocks[MAX_BLOCKS * 2];
    int a_len, b_len, res_len;

    string_to_blocks(a->mantissa, a_blocks, &a_len);
    string_to_blocks(b->mantissa, b_blocks, &b_len);

    // 4. Перемножаем блоки
    multiply_blocks(a_blocks, a_len, b_blocks, b_len, res_blocks, &res_len);

    // 5. Преобразуем результат обратно в строку
    char temp_result[61] = {0};
    blocks_to_string(res_blocks, res_len, temp_result);

    // 6. Вычисляем порядок результата
    // Простое правило: порядки складываются + коррекция на длину
    int exp_a = (a->sign_e == '+') ? a->exponent : -a->exponent;
    int exp_b = (b->sign_e == '+') ? b->exponent : -b->exponent;
    // Простая формула: порядки складываются + коррекция на переполнение
    int result_digits = strlen(temp_result);
    int expected_digits = strlen(a->mantissa) + strlen(b->mantissa);
    int total_exp = exp_a + exp_b;
    // Коррекция на переполнение (если результат короче)
    if (result_digits < expected_digits)
        total_exp -= (expected_digits - result_digits);

    if (total_exp >= 0)
    {
        result->sign_e = '+';
        result->exponent = total_exp;
    }
    else
    {
        result->sign_e = '-';
        result->exponent = -total_exp;
    }
    // 7. Обрезаем до 40 знаков
    if (strlen(temp_result) > 40)
        temp_result[40] = '\0';

    // 8. Копируем мантиссу
    strcpy(result->mantissa, temp_result);

    return 0;
}
