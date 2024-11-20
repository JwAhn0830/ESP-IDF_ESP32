#include <stdio.h>
#include <string.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TEST_LED 5
void uart_init() {
    uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);
}
void send_uart(char *ptr) {
    uart_write_bytes(UART_NUM_0, ptr, strlen(ptr));
}

void uart_task() {
    uint8_t data[128];
    while(1) {
        int length = uart_read_bytes(UART_NUM_0, data, 1024, 20 / portTICK_PERIOD_MS);
        if (length > 0) {
            data[length] = '\0';
            if(strstr((char*)data, "LED OFF") != NULL) {
                gpio_set_level(TEST_LED, 1);
            }
            else if (strstr((char*)data, "LED ON") != NULL) {
                gpio_set_level(TEST_LED, 0);     
            }
            send_uart((char*)data);
        }
    }
}

void app_main(void)
{
    gpio_set_direction(TEST_LED, GPIO_MODE_OUTPUT);
    uart_init();
    send_uart("MCU Confiquration Done\r\n");
    vTaskDelay(pdMS_TO_TICKS(2000));
    xTaskCreate(uart_task, "uart_task", 2048, NULL, 10, NULL);  // UART 출력 태스크 생성
}