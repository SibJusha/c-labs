#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define N 255

int is_symbol (char beg) {
    if (beg == '+' || beg == '*' || beg == '/' || beg == '-') {
        return 1;
    }
    return 0;
}

int is_number (char* beg) {
    char* p = beg;
    while (isdigit(*p)) {
        p++;
    }
    return p - beg;
}

int is_indtf (char* beg) {
    char* p = beg;
    if (!isalpha(*p)) {
        return 0;
    }
    while (isalnum(*p) || *p == '_') {
        p++;
    }
    return p - beg;
}

int simplicity (char* str, int count_args, int brackets) {
    char* start = str;
    if (*str == '(') {
        int br_check = simplicity(str + 1, 0, ++brackets);
        if (br_check == 0) {
            return 0;
        }
        str += br_check + 1;
        if (*str == ')') {
            brackets--;
            str++;
            if (*str != 0 && brackets == 0) {
                return 0;
            }
            if (count_args == 1) {
                return str - start;
            }
        }
        count_args++;
        if (brackets == 0) {
            return 1;
        }
    }
    int check = is_number(str) + is_indtf(str);
    if (check) {
        str += check;
        if (brackets == 0 && *str == 0) {
            return str - start;
        }
        count_args++;
    }

    if (is_symbol(*str) && count_args == 1 && brackets > 0) {
        int check2 = simplicity(str + 1, 1, brackets);
        if (check2 == 0) {
            return 0;
        }
        str += check2 + 1;
        if (*str != ')') {
            return 0;
        }
        return str - start;
    }
    if (count_args == 2 && *str == ')') {
        return str - start;
    }
    return 0;
}

int calculate(char* str) {
    int sum = 0;
    if (isdigit(*str)) {
        int x = 0;
        for (; isdigit(*str); str++) {
            x = x*10 + *(str) - '0';
        }
        sum += x;
    }
    if (*str == '(') {
        sum += calculate(++str);
        for (int brackets = 1; brackets > 0; str++){
            if (*str == '(') {
                brackets++;
            }
            if (*str == ')') {
                brackets--;
            }
        }
    }
    if (*str == '+') {
        sum += calculate(++str);
    }
    if (*str == '*') {
        sum *= calculate(++str);
    }
    if (*str == '-') {
        sum -= calculate(++str);
    }
    if (*str == '/') {
        sum /= calculate(++str);
    }
    return sum;
}

int check_n_replace(char* str, char* str_new, char* var, char* var_value) {
    for (int j = 0; isalnum(str[j]) || str[j] == '_' || var[j] != 0; j++) {
        if (var[j] != str[j]) {
            return -1;
        }
    }
    int i = 0;
    for (; var_value[i] != 0; i++) {
        str_new[i] = var_value[i];
    }
    return i;
}

int main() {
    int n;
    char str[N+1];
    char vars[N][N+1];
    char vars_values[N][N+1];
    scanf("%d\n", &n);
    for (int i = 0; i < n; i++) {
        scanf("%s ", &vars[i]);
        scanf("%s", &vars_values[i]);
        scanf("\n");
    }
    scanf("%s", str);
    if (simplicity(str, 0,0) == 0) {
        printf("INCORRECT");
        return 0;
    }
    char str_new[N+1];
    char* p = str;
    char* q = str_new;
    int only_numbers = 1;
    while (*p != 0) {
        if (isalpha(*p)) {
            if (n == 0) {
                only_numbers = 0;
            }
            for (int k = 0; k < n && isalpha(*p); k++) {
                int stop = check_n_replace(p, q, vars[k], vars_values[k]);
                if (stop != -1) {
                    q += stop;
                    p += strlen(vars[k]);
                }
                if (k + 1 == n && (stop == -1 || isalpha(*p))) {
                    while (isalnum(*p) || *p == '_') {
                        *q = *p;
                        p++;
                        q++;
                        only_numbers = 0;
                    }
                }
            }
        }
        *q = *p;
        p++;
        q++;
    }
    printf("%s", str_new);
    if (only_numbers) {
        int result = calculate(str_new);
        printf("\nresult = %d", result);
    }
    return 0;
}