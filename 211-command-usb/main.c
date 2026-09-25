#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "led.h"
#include "log.h"
#include "device.h"
#include <string.h>

// функции - обработчики команд
void cmd_enable(void)
{
    // включаем светодиод и сообщаем новое состояние
    led_set(true);
        
    LOG_INF("led %s\n", led_is_on() ? "on" : "off");
}

void cmd_disable(void)
{
    // выключаем светодиод и сообщаем новое состояние
    led_set(false);
        
    LOG_INF("led %s\n", led_is_on() ? "on" : "off");
}

void cmd_info(void)
{
    device_info();// печатаем паспорт устройства
}

void cmd_version(void)
{
    log_version();// печатаем строку журнала о версии прошивки
}
void cmd_ping(void)
{
    printf("pong\n");
}
// таблица комманд
typedef void (*command_handler_t)(void);
struct command_t
{
    const char *name;
    command_handler_t handler;
};

const struct command_t commands[] = {
    { "enable", cmd_enable },
    { "disable", cmd_disable },
    { "info", cmd_info },
    { "version", cmd_version },
    { "ping", cmd_ping },
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))
// строковая команда
#define LINE_SIZE 32

char line[LINE_SIZE];
uint line_length = 0;
void handle_command(const char *command)
{
    for (uint i = 0; i < COMMAND_COUNT; i++)
    {
        if (strcmp(command, commands[i].name) == 0)
        {
            if (commands[i].handler != NULL)
            {
                commands[i].handler();
            }

            return;
        }
    }

    LOG_ERR("unknown command: %s\n", command);
}

void read_line(void)
{
    int symbol = getchar_timeout_us(0);

    if (symbol == PICO_ERROR_TIMEOUT)
    {
        return;
    }

    if (symbol == '\r' || symbol == '\n')
    {
        putchar('\n');
        line[line_length] = '\0';

        if (line_length > 0)
        {
            LOG_DBG("got %s\n", line);
            handle_command(line);
        }

        line_length = 0;
        return;
    }

    if (line_length + 1 < LINE_SIZE)
    {
        line[line_length] = (char)symbol;
        line_length = line_length + 1;
        putchar(symbol);
    }
}

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
       read_line();
      // LOG_DBG("got full %s\n", line);
      // handle_command(line);
    }
}