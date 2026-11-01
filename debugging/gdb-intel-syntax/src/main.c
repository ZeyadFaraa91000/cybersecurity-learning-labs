#include <stdio.h>

int add_numbers(int a, int b)
{
    int result = a + b;
    return result;
}

int main(void)
{
    int x = 10;
    int y = 20;

    int sum = add_numbers(x, y);

    printf("Result = %d\n", sum);

    return 0;
}
