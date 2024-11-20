#include <stdio.h>
#include "freertos/FreeRTOS.h" //for delay,mutexs,semphrs rtos operations
#include "driver/gpio.h"
#include "esp_wifi.h"
#include "nvs_flash.h" //non volatile storage
#include "esp_event.h" //for wifi event
//#include "lwip/sys.h" //system applications for light weight ip apps
//#include "lwip/err.h" //light weight ip packets error handling
//#include "esp_system.h" //esp_init funtions esp_err_t 

#define ON 0
#define OFF 1
#define SSID "test"
#define PASSWORD "test"

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data) {
  if (event_id == WIFI_EVENT_STA_START) {
    printf("Connecting WiFi...\r\n");
  }
  else if (event_id == WIFI_EVENT_STA_CONNECTED) {
    printf("WiFi CONNECTED\r\n");
    gpio_set_level(5, ON);
  }
  else if (event_id == WIFI_EVENT_STA_DISCONNECTED){
    gpio_set_level(5, OFF);
    printf("WiFi DISCONNECTED Retrying to Connect...\r\n");
    esp_wifi_connect(); 
  }
}
void init_wifi(){
  esp_netif_init(); //init wifi interface
  esp_event_loop_create_default();
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&wifi_init_config);

  esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
  esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

  wifi_config_t wifi_config = {
    .sta = {
      .ssid = SSID,
      .password = PASSWORD,
    },
  };

  esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
  esp_wifi_start(); // start WiFi driver 
  esp_wifi_set_mode(WIFI_MODE_STA); // set wifi mode as STA

  /*
  once this called, the wifi driver start to scan and coneect wifi
  when connection is successful, WIFI_EVENT_STA_CONNECTED is generated
  */ 
  esp_wifi_connect(); 
}
void app_main() 
{
  gpio_set_direction(5, GPIO_MODE_OUTPUT);
  gpio_set_level(5, OFF);

  nvs_flash_init();
  init_wifi();

}