

#include <stdio.h>
#include <stdint.h>

uint16_t u16_Buff[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
uint16_t counter = 3;
uint8_t u8_IdxBuffHead = 9;
uint8_t u8_IdxBuffTail = 0;

uint16_t startIdx = 0;
int main()
{
    startIdx = u8_IdxBuffHead - counter;
    while (u8_IdxBuffHead != u8_IdxBuffTail)
    {

        printf("[%d]\n", 134 % 128);
        u8_IdxBuffTail = (startIdx + 1) % 11;
        startIdx++;
    }
}
// End of File