
#include "operations.h"

size_t max(size_t a, size_t b)
{
    return a > b ? a : b;
}

size_t min(size_t a, size_t b)
{
    return a > b ? b : a;
}

void *add_minus(char *str, size_t len)
{
    str = realloc(str, sizeof(char) * (len + 2));
    for (size_t i = 0; i < len + 1; i++)
        str[len + 1 - i] = str[len - i];
    str[0] = '-';
}

char *addition_int_int(char *a, char *b)
{
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);

    size_t len_res = max(len_a, len_b);
    size_t len_min = min(len_a, len_b);

    char *res = malloc(sizeof(char) * (len_res + 1));
    res[len_res] = 0;

    size_t retenue = 0;
    size_t i;
    for (i = 0; i < len_min; i++)
    {
        res[len_res - i - 1] = a[len_a - i - 1] - '0';
        res[len_res - i - 1] += b[len_b - i - 1] + retenue;
        if (res[len_res - i - 1] > '9')
        {
            retenue = 1;
            res[len_res - i - 1] -= 10;
        }
        else
            retenue = 0;
    }
    if (len_a > len_b)
    {
        while (i < len_res)
        {
            res[len_res - i - 1] = a[len_a - i - 1] + retenue;
            i++;
        }
    }
    else
    {
        while (i < len_res)
        {
            res[len_res - i - 1] = b[len_b - i - 1] + retenue;
            i++;
        }
    }

    if (len_a == len_b && retenue > 0)
    {
        res = realloc(res, sizeof(char) * (len_res + 2));
        for (i = len_res + 1; i > 0; i--)
            res[i] = res[i - 1];
        res[0] = retenue + '0';
    }

    return res;
}

char *addition_float_int(char *a, char *b)
{
    return NULL;
}

char *addition_float_float(char *a, char *b)
{
    return NULL;
}

size_t index_str(char *str, char value)
{
    size_t i = 0;
    while (str[i])
    {
        if (str[i] == value)
            return i;
        i++;
    }
    return -1;
}

char *addition(char *a, char *b)
{
    size_t negative = a[0] == '-' ^ b[0] == '-';
    size_t a_is_float = index_str(a, '.');
    size_t b_is_float = index_str(b, '.');
    char *new_a = a + (a[0] == '-' ? 1 : 0);
    char *new_b = b + (b[0] == '-' ? 1 : 0);

    char *res;
    if (a_is_float == -1) // a int
    {
        if (b_is_float == -1) // b int
            res = addition_int_int(new_a, new_b);
        else // b float
            res = addition_float_int(new_b, new_a);
    }
    else // a float
    {
        if (b_is_float == -1) // b int
            res = addition_float_int(new_a, new_b);
        else // b float
            res = addition_float_float(new_a, new_b);
    }
    if (negative)
        add_minus(res, strlen(res));

    return res;
}
