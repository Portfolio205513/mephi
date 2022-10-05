#include <stdio.h>
#include <stdint.h>

int divisibility_by_11(int32_t n)
{
     int32_t sum_t;
     int8_t flag;

     while(n / 10 != 0)
     {
         sum_t = 0;

         flag = 1;
         while(n)
         {
             sum_t += flag * (n % 10);
             n /= 10;
             flag *= -1;
         }
         n = sum_t;
     }
     if (n == 0)
         return 1;
     else
         return 0;
}

int main()
{
    int32_t n;
    
    printf("Enter integer number:\n");
    scanf("%d", &n);
    if (divisibility_by_11(n))
        printf("Number is divisible by 11\n");
    else
        printf("Number is not divisible by 11\n");
    return 0;
}