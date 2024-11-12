#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUILEDTIN_LED 5

void gpio_toggle_level(gpio_num_t gpio_num) {
    static bool level = false;

    gpio_set_level(gpio_num, level);
    level = !level;
}

void app_main(void)
{
    gpio_set_direction(BUILEDTIN_LED, GPIO_MODE_OUTPUT);

    while(1) {
        gpio_toggle_level(BUILEDTIN_LED);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}