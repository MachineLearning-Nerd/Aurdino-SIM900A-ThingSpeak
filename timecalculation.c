#include <stdio.h>
unsigned int CalcDays(int iMonth, int iDay, int iYear)
{
    unsigned long startdate = (1 + (153 * (1 + 12 * ((14 - 1) / 12) - 3) + 2) / 5 + 365 *
        (18 + 4800 - ((14 - 1) / 12)) + (18 + 4800 - ((14 - 1) / 12)) / 4 - 32083);
    unsigned long lastdate =   (iDay + (153 * (iMonth + 12 * ((14 - iMonth) / 12) - 3) + 2) / 5 + 365 *
        (iYear + 4800 - ((14 - iMonth) / 12)) + (iYear + 4800 - ((14 - iMonth) / 12)) / 4 - 32083);
        
    return (lastdate-startdate);
}
int main(void)
{
    printf("%d\n", CalcDays(1, 2, 19));
    return getchar();
}