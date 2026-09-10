#include "pico/stdlib.h"
#include "hardware/gpio.h"

// led & button
const uint LED_PIN = 25;
const uint BUTTON_PIN = 15;
int main()
{
    // весь дальнейший код пишем здесь
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    bool led = false;
    bool previous = false;
    while (1)
    {
        // код вкл/выкл светодиода от кнопки
       bool current = gpio_get(BUTTON_PIN);
       if (previous == true && current ==false) {
          led = !led;
          gpio_put(LED_PIN, led);
       }
       previous = current;
    }
}