#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "../botao/botao.h"
#include "../led/led.h"
#include "../oled/oled.h"


#define TRIGGER_PIN GPIO_NUM_2
#define ECHO_PIN GPIO_NUM_3

static const char* TAG = "distance_sensor";

// Variáveis de controle
int measurement_enabled = 0;
char buffer[20];
float distance_cm = 0;
int first_stop = 1;

void distance_measurement_task() {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << TRIGGER_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << ECHO_PIN);
    gpio_config(&io_conf);


    while (1) {
        // Enviar pulso de disparo
        gpio_set_level(TRIGGER_PIN, 0);
        ets_delay_us(4);
        gpio_set_level(TRIGGER_PIN, 1);
        ets_delay_us(10);
        gpio_set_level(TRIGGER_PIN, 0);

        // Aguardar o início do eco
        uint32_t start_time = esp_timer_get_time();
        while (gpio_get_level(ECHO_PIN) == 0) {
        }

        // Medir a duração do eco
        start_time = esp_timer_get_time();
        uint32_t echo_time = 0;
        while (gpio_get_level(ECHO_PIN) == 1) {
            echo_time = esp_timer_get_time();
        }

        if (gpio_get_level(ECHO_PIN) == 0) {
            // Calcular a distância em centímetros
            distance_cm = (echo_time - start_time) * 0.0343 / 2.0;
            if(get_calibration()) {
                
                sprintf(buffer, "%.2f cm", distance_cm);
                set_calibration_text(4, true, buffer, true);
                ESP_LOGI(TAG, "Distância: %.2f cm", distance_cm);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Aguardar 100 milissegundos antes de verificar novamente o botão e realizar medições
    }
}

float get_distance(){
    return distance_cm;
}

