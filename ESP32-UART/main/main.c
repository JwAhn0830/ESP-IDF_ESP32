#include <stdio.h>
#include <string.h>
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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

void uart_task() {
    const char* message = "hello world!\n";
    while(1) {
        uart_write_bytes(UART_NUM_0, message, strlen(message));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    uart_init();
    xTaskCreate(uart_task, "uart_task", 2048, NULL, 10, NULL);  // UART 출력 태스크 생성
}