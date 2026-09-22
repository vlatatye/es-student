#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "led.h"
#include "log.h"
#include "device.h"

// button

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

void handle_command(int command)
{
    if (command == 'e')
    {
        led_set(true);
        
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
    else if (command == 'd')
    {
        led_set(false);
        
        LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }
        else if (command == 'v')
    {
        log_version();
    }
        else if (command == 'i')
    {
        device_info();
    }
    else
    {
        
        LOG_ERR("unknown command: %c\n", command);
    }
}

int main()
{
    // весь дальнейший код пишем здесь
    stdio_init_all();
    
    led_init();
    gpio_pull_up(BUTTON_PIN);
    
    bool previous = false;
    while (1)
    {
        // код вкл/выкл светодиода от кнопки,
        // прогр подавление дребезга
       bool current = get_button_debounce(BUTTON_PIN);
       if (previous == true && current ==false) {
          
          led_toggle();
         
          LOG_INF("led %s\n", led_is_on() ? "on" : "off");
       }
       previous = current;
       int command = getchar_timeout_us(0);

       if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }
       LOG_DBG("got %c\n", command);
       handle_command(command);
    }
}