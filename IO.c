#include "IO.h"
int count_letter(char *str, char ch);
void add_null_start_float(char *str, size_t *len_str);
int input_float(long_number_t *num, char num_str[41], size_t *len_num);
int input_int(long_number_t *num, char num_str[41], size_t *len_num);
void find_mantissa_float(char *str, char mantissa[40]);
void find_exp(char *str, char *sing_e, int *exponent);
int only_zeros_before_dot(const char *str);

int parse_input(long_number_t *first_num, long_number_t *second_num, size_t *len_first_num, size_t *len_second_num)
{
    char first_num_str[41];
    char second_num_str[41];
    char entry;

    printf("Введите первое число для (деления или умножения) в формате +-123.123, обязательно знак\n");
    printf("+---------1---------2---------3---------4\n");

    if (scanf("%38s%c", first_num_str, &entry) != 2 || entry != '\n')
    {
        printf("fail scanf first num\n");
        return 1;
    }
    *len_first_num = strlen(first_num_str);

    if (strchr(first_num_str, '.') == NULL && (strchr(first_num_str, 'e') == NULL || strchr(first_num_str, 'E') == NULL))
        input_int(first_num, first_num_str, len_first_num);
    if (strchr(first_num_str, '.') != NULL || (strchr(first_num_str, 'e') != NULL || strchr(first_num_str, 'E') != NULL))
    {
        input_float(first_num, first_num_str, len_first_num);
    }

    printf("Введите второе число для (деления или умножения) в формате +-123.123, обязательно знак\n");
    printf("+---------1---------2---------3---------4\n");

    if (scanf("%38s%c", second_num_str, &entry) != 2 || entry != '\n')
    {
        printf("fail scanf second num\n");
        return 1;
    }
    *len_second_num = strlen(second_num_str);
    if (strchr(second_num_str, '.') == NULL && (strchr(second_num_str, 'e') == NULL || strchr(second_num_str, 'E') == NULL))
        input_int(second_num, second_num_str, len_second_num);
    if (strchr(second_num_str, '.') != NULL || (strchr(second_num_str, 'e') != NULL || strchr(second_num_str, 'E') != NULL))
        input_float(second_num, second_num_str, len_second_num);
    return 0;
}

int input_int(long_number_t *num, char num_str[41], size_t *len_num)
{

    if (strchr(num_str, '+') != NULL || strchr(num_str, '-') != NULL)
    {
        num->sign_m = num_str[0];
        strcpy(num->mantissa, num_str + 1);
    }

    if (strchr(num_str, '.') == NULL && (strchr(num_str, 'e') == NULL || strchr(num_str, 'E') == NULL))
    {
        num->sign_e = '+';
        num->exponent = (*len_num) - 2;
    }
    printf("sign_m = %c\nmantisaa = %s\nsign_e = %c\nexponent = %d\n", num->sign_m, num->mantissa, num->sign_e, num->exponent);

    return 0;
}

int input_float(long_number_t *num, char num_str[41], size_t *len_num)
{

    if (strchr(num_str, '+') != NULL || strchr(num_str, '-') != NULL)
        num->sign_m = num_str[0];
    if (count_letter(num_str, '.') > 1 || count_letter(num_str, 'e') > 1 || count_letter(num_str, 'E') > 1)
        return ERR_FLOAT;

    if ((strchr(num_str, '.') - num_str) == 1) // если нет нуля (+.0123)
    {
        add_null_start_float(num_str, len_num);
        find_mantissa_float(num_str, num->mantissa);
        find_exp(num_str, &num->sign_e, &num->exponent);
        printf("sign_m = %c\nmantisaa = %s\nsign_e = %c\nexponent = %d\n", num->sign_m, num->mantissa, num->sign_e, num->exponent);
    }
    else
    {
        find_mantissa_float(num_str, num->mantissa);
        find_exp(num_str, &num->sign_e, &num->exponent);
        printf("sign_m = %c\nmantisaa = %s\nsign_e = %c\nexponent = %d\n", num->sign_m, num->mantissa, num->sign_e, num->exponent);
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
    int found_non_zero = 0; // Флаг, что нашли значащую цифру

    if (strchr(str, 'e') == NULL && strchr(str, 'E') == NULL)
    {
        for (int i = 0; str[i] != '\0'; i++)
            if (isdigit(str[i]))
                if (found_non_zero || str[i] != '0')
                {
                    mantissa[j] = str[i];
                    j++;
                    found_non_zero = 1;
                }

        if (j == 0)
        {
            mantissa[0] = '0';
            j = 1;
        }
    }
    else
    {
        int flag_E = 0;
        int j = 0;
        for (size_t i = 0; str[i] != '\0' && flag_E == 0; i++)
        {
            if (isdigit(str[i]))
            {
                mantissa[j] = str[i];
                j++;
            }
            else if (str[i] == 'E' || str[i] == 'e')
                flag_E = 1;
        }
        mantissa[j] = '\0';
    }
}

void find_exp(char *str, char *sing_e, int *exponent)
{
    if (only_zeros_before_dot(str) && (strchr(str, 'E') == NULL || strchr(str, 'e') == NULL))
    {
        *sing_e = '-';
        int count_null = 0;
        for (size_t i = strchr(str, '.') - str; str[i] != '\0'; i++)
            if (str[i] == '0')
                count_null++;
        *exponent = count_null + 1;
    }
    else if (strchr(str, 'E') == NULL && strchr(str, 'e') == NULL) // если без ешки
    {
        *sing_e = '+';
        int count = 0;
        int flag_dot = 0;
        for (size_t i = 0; str[i] != '\0' && flag_dot == 0; i++)
            if (str[i] != '.' && str[i] != '-' && str[i] != '+')
                count++;
            else if (str[i] == '.')
                flag_dot = 1;
        *exponent = count;
    }
    else
    {
        if (strchr(str, '.') != NULL && strchr(str, 'E') != NULL)
        {
            *sing_e = str[(strchr(str, 'E')) - str + 1];
            *exponent = atoi(strchr(str, 'E') + 2);
        }
        else if (strchr(str, '.') != NULL && strchr(str, 'e') != NULL)
        {
            *sing_e = str[(strchr(str, 'e')) - str + 1];
            *exponent = atoi(strchr(str, 'e') + 2);
        }
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

void output_num(long_number_t result_num)
{
    printf("%c0.%s E %c%d\n", result_num.sign_m, result_num.mantissa, result_num.sign_e, result_num.exponent);
}
