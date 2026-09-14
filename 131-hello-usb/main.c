#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"


int main()
{
    // весь дальнейший код пишем здесь
    stdio_init_all();
    
    while (1)
    {
        // код выдачи сообщения
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}