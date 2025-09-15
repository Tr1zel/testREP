#include "IO.h"
int count_letter(char *str, char ch);
void add_null_start_float(char *str, size_t *len_str);
int input_float(long_number_t *num, char num_str[41], size_t *len_num);
int input_int(long_number_t *num, char num_str[41]);
void find_mantissa_float(char *str, char mantissa[40]);
void find_exp(char *str, char *sing_e, int *exponent);
int only_zeros_before_dot(const char *str);
void add_sign_if_missing(char *str, size_t *len_str);

int parse_input(long_number_t *first_num, long_number_t *second_num, size_t *len_first_num, size_t *len_second_num)
{
    char first_num_str[41];
    char second_num_str[41];
    char entry;

    // printf("Введите первое число для (деления или умножения) в формате +-123.123, обязательно знак\n");
    // printf("+---------1---------2---------3---------4\n");

    if (scanf("%38s%c", first_num_str, &entry) != 2 || entry != '\n')
    {
        printf("fail scanf first num\n");
        return 1;
    }
    *len_first_num = strlen(first_num_str);
    add_sign_if_missing(first_num_str, len_first_num);

    if (strchr(first_num_str, '.') == NULL && (strchr(first_num_str, 'e') == NULL || strchr(first_num_str, 'E') == NULL))
        input_int(first_num, first_num_str);
    if (strchr(first_num_str, '.') != NULL || (strchr(first_num_str, 'e') != NULL || strchr(first_num_str, 'E') != NULL))
    {
        input_float(first_num, first_num_str, len_first_num);
    }

    // printf("Введите второе число для (деления или умножения) в формате +-123.123, обязательно знак\n");
    // printf("+---------1---------2---------3---------4\n");

    if (scanf("%38s%c", second_num_str, &entry) != 2 || entry != '\n')
    {
        printf("fail scanf second num\n");
        return 1;
    }
    *len_second_num = strlen(second_num_str);
    add_sign_if_missing(second_num_str, len_second_num);
    if (strchr(second_num_str, '.') == NULL && (strchr(second_num_str, 'e') == NULL || strchr(second_num_str, 'E') == NULL))
        input_int(second_num, second_num_str);
    if (strchr(second_num_str, '.') != NULL || (strchr(second_num_str, 'e') != NULL || strchr(second_num_str, 'E') != NULL))
        input_float(second_num, second_num_str, len_second_num);
    return 0;
}

int input_int(long_number_t *num, char num_str[41])
{
    // Теперь знак всегда есть (добавляется автоматически)
    num->sign_m = num_str[0];

    // Проверяем длину мантиссы
    size_t mantissa_len = strlen(num_str + 1);
    if (mantissa_len > 30)
    {
        // Обрезаем до 30 цифр
        strncpy(num->mantissa, num_str + 1, 30);
        num->mantissa[30] = '\0';
        printf("Предупреждение: число обрезано до 30 цифр\n");
    }
    else
    {
        strcpy(num->mantissa, num_str + 1);
    }

    if (strchr(num_str, '.') == NULL && (strchr(num_str, 'e') == NULL || strchr(num_str, 'E') == NULL))
    {
        num->sign_e = '+';
        // Порядок = количество цифр в числе
        num->exponent = strlen(num->mantissa);
    }
    // printf("sign_m = %c\nmantisaa = %s\nsign_e = %c\nexponent = %d\n", num->sign_m, num->mantissa, num->sign_e, num->exponent);

    return 0;
}

int input_float(long_number_t *num, char num_str[41], size_t *len_num)
{
    // Теперь знак всегда есть (добавляется автоматически)
    num->sign_m = num_str[0];
    if (count_letter(num_str, '.') > 1 || count_letter(num_str, 'e') > 1 || count_letter(num_str, 'E') > 1)
        return ERR_FLOAT;

    if ((strchr(num_str, '.') - num_str) == 1) // если нет нуля (+.0123)
    {
        add_null_start_float(num_str, len_num);
        find_mantissa_float(num_str, num->mantissa);
        find_exp(num_str, &num->sign_e, &num->exponent);
        // printf("sign_m = %c\nmantisaa = %s\nsign_e = %c\nexponent = %d\n", num->sign_m, num->mantissa, num->sign_e, num->exponent);
    }
    else
    {
        find_mantissa_float(num_str, num->mantissa);
        find_exp(num_str, &num->sign_e, &num->exponent);
        // printf("sign_m = %c\nmantisaa = %s\nsign_e = %c\nexponent = %d\n", num->sign_m, num->mantissa, num->sign_e, num->exponent);
    }
    return 0;
}

int count_letter(char *str, char ch)
{
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] == ch)
            count++;
    return count;
}

void add_null_start_float(char *str, size_t *len_str)
{
    (*len_str) += 2;
    char new_str[43]; // проверить что длина мантиссы больше чем предел
    new_str[0] = str[0];
    new_str[1] = '0';
    strcat(new_str, (str) + 1);
    strcpy(str, new_str);
}

void find_mantissa_float(char *str, char mantissa[40])
{
    int j = 0;
    int found_non_zero = 0;

    // Находим конец числовой части
    int end_pos = strlen(str);
    char *e_pos = strchr(str, 'E');
    if (e_pos == NULL)
        e_pos = strchr(str, 'e');
    if (e_pos != NULL)
        end_pos = e_pos - str;

    // Извлекаем все цифры, убирая ведущие нули
    for (int i = 0; i < end_pos && j < 30; i++)
    {
        if (isdigit(str[i]))
        {
            if (found_non_zero || str[i] != '0')
            {
                mantissa[j] = str[i];
                j++;
                found_non_zero = 1;
            }
        }
    }

    if (j == 0)
    {
        mantissa[0] = '0';
        j = 1;
    }

    mantissa[j] = '\0';
}

void find_exp(char *str, char *sign_e, int *exponent)
{
    // 1. Найдём часть до 'E' (если есть)
    char *e_pos = strchr(str, 'E');
    if (!e_pos)
        e_pos = strchr(str, 'e');

    int explicit_exp = 0;
    if (e_pos)
    {
        explicit_exp = atoi(e_pos + 1);
    }

    // 2. Найдём точку
    char *dot_pos = strchr(str, '.');

    // 3. Ищем первую значащую цифру
    int idx_first_digit = -1;
    for (int i = 0; str[i] != '\0' && str + i != e_pos; i++)
    {
        if (isdigit(str[i]) && str[i] != '0')
        {
            idx_first_digit = i;
            break;
        }
    }

    if (idx_first_digit == -1)
    {
        // все нули
        *sign_e = '+';
        *exponent = 0;
        return;
    }

    // 4. Считаем экспоненту
    int exp_val = explicit_exp;

    if (dot_pos)
    {
        if (idx_first_digit < (dot_pos - str))
        {
            // первая цифра до точки
            exp_val += (dot_pos - str) - idx_first_digit;
        }
        else
        {
            // первая цифра после точки
            exp_val -= (idx_first_digit - (dot_pos - str));
        }
    }
    else
    {
        // точки нет
        exp_val += (strlen(str) - idx_first_digit);
    }

    // 5. Записываем знак/значение
    if (exp_val >= 0)
    {
        *sign_e = '+';
        *exponent = exp_val;
    }
    else
    {
        *sign_e = '-';
        *exponent = -exp_val;
    }
}

int only_zeros_before_dot(const char *str)
{
    char *dot_pos = strchr(str, '.');
    for (const char *p = str; p < dot_pos; p++)
        if (*p != '0' && *p != '+' && *p != '-')
            return 0;
    return 1;
}

void add_sign_if_missing(char *str, size_t *len_str)
{
    // Проверяем, есть ли знак в начале строки
    if (str[0] != '+' && str[0] != '-')
    {
        // Сдвигаем строку вправо на 1 позицию
        for (int i = *len_str; i >= 0; i--)
        {
            str[i + 1] = str[i];
        }
        // Добавляем знак +
        str[0] = '+';
        (*len_str)++;
    }
}

void remove_trailing_zeros(char *mantissa)
{
    int len = strlen(mantissa);
    // Убираем нули с конца, оставляя минимум одну цифру
    while (len > 1 && mantissa[len - 1] == '0')
    {
        mantissa[len - 1] = '\0';
        len--;
    }
}

void output_num(long_number_t result_num)
{
    // Убираем лишние нули перед выводом
    remove_trailing_zeros(result_num.mantissa);
    printf("%c0.%s E %c%d\n", result_num.sign_m, result_num.mantissa, result_num.sign_e, result_num.exponent);
}
