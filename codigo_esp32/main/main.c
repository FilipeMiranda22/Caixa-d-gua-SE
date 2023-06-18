#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "config/temperatura/temperatura.h"
#include "config/oled/oled.h"
#include "ssd1306.h"
#include "esp_log.h"
#include "font8x8_basic.h"
#include "config/botao/botao.h"
#include "config/distancia/distancia.h"
#include "config/led/led.h"
#include "config/atuador/atuador.h"

SSD1306_t dev;

static const char* TAG = "ESP32";

void app_main(void)
{
    init_led();
    init_oled(); 
    init_atuador();
    xTaskCreate(button_task, "button_task", 4096, NULL, 1, NULL);
    xTaskCreate(init_temperatura, "temperature_task", 2048, NULL, 5, NULL);
    ESP_LOGI(TAG, "Inicializando sensor de distância HC-SR04");
    xTaskCreate(distance_measurement_task, "distance_task", 4096, NULL, 2, NULL);
}
