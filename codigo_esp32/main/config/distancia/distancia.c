#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "../botao/botao.h"
#include "../led/led.h"

#define TRIGGER_PIN GPIO_NUM_2
#define ECHO_PIN GPIO_NUM_3

#define LED_RED_PIN GPIO_NUM_10
#define LED_GREEN_PIN GPIO_NUM_1

static const char* TAG = "distance_sensor";

// Variáveis de controle
int measurement_enabled = 0;
float empty_distance = 0;
float full_distance = 0;
float distance_cm = 0;
int first_stop = 1;

void distance_measurement_task() {
    gpio_pad_select_gpio(LED_RED_PIN);
    gpio_set_direction(LED_RED_PIN, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(LED_GREEN_PIN);
    gpio_set_direction(LED_GREEN_PIN, GPIO_MODE_OUTPUT);

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

        if (get_button_pressed() && !measurement_enabled) {
            ESP_LOGI(TAG, "Iniciando medição de distância");
            measurement_enabled = 1;
            set_button_pressed(0);
        } else if (get_button_pressed() && measurement_enabled) {
            // Botão pressionado e medição ativada
            ESP_LOGI(TAG, "Parando medição de distância");
            ESP_LOGI(TAG, "Distancia vazia: %.2f cm", empty_distance);
            ESP_LOGI(TAG, "Distancia cheia: %.2f cm", full_distance);
            first_stop = 0;
            measurement_enabled = 0;
            set_button_pressed(0);
        }


        if (measurement_enabled){

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

                if (empty_distance == 0 && first_stop == 1) {
                    empty_distance = distance_cm;
                } else if (empty_distance != 0 && first_stop == 0 && full_distance == 0) {
                    full_distance = distance_cm;
                }
                
                ESP_LOGI(TAG, "Distância: %.2f cm", distance_cm);
                
                // Verificar se a distância está próxima à variável de controle
                if (measurement_enabled && first_stop != 1 && distance_cm >= empty_distance) {
                    ESP_LOGI(TAG, "ASCENDI LED VERMELHO");
                    turn_on_led(LED_RED_PIN); // Acender o LED vermelho
                } else {
                    turn_off_led(LED_RED_PIN); // Apagar o LED vermelho
                }

                // Verificar se a distância está próxima à variável de controle
                if (measurement_enabled && first_stop != 1 && distance_cm <= (full_distance + 1)) {
                    ESP_LOGI(TAG, "ASCENDI LED VERDE");
                    turn_on_led(LED_GREEN_PIN); // Acender o LED vermelho
                } else {
                    turn_off_led(LED_GREEN_PIN); // Apagar o LED verde
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Aguardar 100 milissegundos antes de verificar novamente o botão e realizar medições
    }
}

