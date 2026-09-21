#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// led & button
const uint LED_PIN = 25;
const uint BUTTON_PIN = 15;
const uint DEBOUNCE_MS = 20;

bool get_button_debounce(uint pin) //прогр. устранение дребезга
{   //считываем пин 2 раза через задержку 20 мс
    bool state = gpio_get(pin);
    sleep_ms(DEBOUNCE_MS);
    return state && gpio_get(pin);
    // если оба раза совпало - выдаем значение через "и"
}//уровень считается высоким, только если он
// высок сейчас и всё ещё высок
// через двадцать миллисекунд
void set_led(bool on) // упр светодиодом и вывод состояния в терминал
{
    gpio_put(LED_PIN, on);
    printf("led %s\n", on ? "on" : "off");
}
bool handle_command(int command, bool led)
{
    if (command == 'e')
    {
        led = true;
        set_led(led);
    }
    else if (command == 'd')
    {
        led = false;
        set_led(led);
    }
    else
    {
        printf("unknown command: %c\n", command);
    }

    return led;
}
int main()
{
    // весь дальнейший код пишем здесь
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    bool led = false;
    bool previous = false;
    while (1)
    {
        // код вкл/выкл светодиода от кнопки,
        // прогр подавление дребезга
       bool current = get_button_debounce(BUTTON_PIN);
       if (previous == true && current ==false) {
          led = !led;
          set_led(led);
       }
       previous = current;
       int command = getchar_timeout_us(0);

       if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }

       led = handle_command(command, led);
    }
}