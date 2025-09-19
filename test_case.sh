#!/bin/bash

# Цвета для вывода
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Компиляция программы
echo -e "${BLUE}Компиляция программы...${NC}"
gcc -Wall -Werror -Wextra -std=c99 *.c -o app.exe
if [ $? -ne 0 ]; then
    echo -e "${RED}Ошибка компиляции!${NC}"
    exit 1
fi

# Создание тестового файла
cat > test_cases.txt << 'EOF'
# ТЕСТЫ ДЛЯ УМНОЖЕНИЯ ДЛИННЫХ ЧИСЕЛ
# Формат: 
# input1
# input2
# expected_output

123
456
+0.56088 E +5

0
123456
+0.0 E +0

1
1
+0.1 E +1

100
0.05
+0.5 E +1

0.001
0.001
+0.1 E -5

-123
456
-0.56088 E +5

123
-456
-0.56088 E +5

-123
-456
+0.56088 E +5

1.234E+2
5.678E+1
+0.7006652 E +4

1.0E-5
2.0E-3
+0.2 E -7

999
999
+0.998001 E +6

1234
5678
+0.7006652 E +7

10000
10000
+0.1 E +9

0.25
4
+0.1 E +1

0.333
3
+0.999 E +0

12.34
56.78
+0.7006652 E +3

-999
999
-0.998001 E +6

-12.34
-56.78
+0.7006652 E +3

-1.5E+2
2.0E+1
-0.3 E +4

1.0E+10
1.0E+10
+0.1 E +21

1.0E-10
1.0E-10
+0.1 E -19

9.999E+9
9.999E+9
+0.99980001 E +20

0.000001
0.000001
+0.1 E -11

1.234E-5
5.678E-3
+0.7006652 E -7

9999999999
1
+0.9999999999 E +10

0.0000000001
10
+0.1 E -8

1.23456789
9.87654321
+0.121932631112635269 E +2

123.456
789.012
+0.97408233472 E +5

1.23456789123456789123456789123456789123456789
2
+0.246913578246913578246913578246913578246913578 E +1

2.5E+3
4.0E+2
+0.1 E +6

7.89E-4
1.23E-2
+0.97047 E -5

-9.876E+5
1.234E-3
-0.12186984 E +3
EOF

echo -e "${BLUE}Запуск тестов...${NC}"
echo "=========================================="

total_tests=0
passed_tests=0

# Чтение тестовых случаев
while IFS= read -r input1 && IFS= read -r input2 && IFS= read -r expected; do
    # Пропускаем комментарии и пустые строки
    if [[ "$input1" =~ ^# ]] || [[ -z "$input1" ]]; then
        continue
    fi
    
    total_tests=$((total_tests + 1))
    
    # Запуск программы с автоматическим вводом двух чисел
    actual=$(echo -e "$input1\n$input2" | ./app.exe 2>/dev/null)
    
    # Убираем лишние пробелы и переводы строк для сравнения
    actual_clean=$(echo "$actual" | tr -d ' ' | tr -d '\n')
    expected_clean=$(echo "$expected" | tr -d ' ' | tr -d '\n')
    
    # Сравнение результатов
    if [ "$actual_clean" = "$expected_clean" ]; then
        echo -e "TEST $total_tests: ${GREEN}PASSED${NC}"
        echo "  Input 1:  $input1"
        echo "  Input 2:  $input2"
        echo "  Expected: $expected"
        echo "  Actual:   $actual"
        passed_tests=$((passed_tests + 1))
    else
        echo -e "TEST $total_tests: ${RED}FAILED${NC}"
        echo "  Input 1:  $input1"
        echo "  Input 2:  $input2"
        echo "  Expected: $expected"
        echo "  Actual:   $actual"
    fi
    echo "------------------------------------------"
    
    # Пропускаем пустую строку между тестами
    IFS= read -r empty_line
    
done < test_cases.txt

# Удаляем временные файлы
rm -f test_cases.txt

echo "=========================================="
echo -e "${BLUE}РЕЗУЛЬТАТЫ:${NC}"
echo -e "Пройдено: ${GREEN}$passed_tests${NC} / ${BLUE}$total_tests${NC}"

if [ $passed_tests -eq $total_tests ]; then
    echo -e "${GREEN}Все тесты пройдены успешно!${NC}"
    exit 0
else
    echo -e "${RED}Есть непройденные тесты!${NC}"
    exit 1
fi