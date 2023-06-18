#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "../distancia/distancia.h"
#include "../led/led.h"
#include "ssd1306.h"
#include "../oled/oled.h"

#define BUTTON_PIN GPIO_NUM_0

char buffer[20]; 

int button_pressed = 0;
float empty_distance = 0;
float full_distance = 0;
int first_press = 1;
int key_red = 0;
int key_green = 0;

SSD1306_t dev;

void button_task() {
    gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    while (1) {
        if (gpio_get_level(BUTTON_PIN) == 0 && !button_pressed) {
            if (first_press){
                ssd1306_clear_screen(&dev, false);
                set_calibration_text(3, true, "Calibrando");
                vTaskDelay(pdMS_TO_TICKS(2000));
                ssd1306_clear_line(&dev,3,false);
                set_calibration_text(1, true, "Medir distancia");
                set_calibration_text(2, true, "Recipiente vazio");
                set_calibration_text(4, true, "Pressione o");
                set_calibration_text(5, true, "botao novamente!");
                first_press = 0;
            }
            else{
                ESP_LOGI("BOTAO", "APERTEI");
                button_pressed = 1;
                if (empty_distance == 0) {
                    empty_distance = get_distance();
                    ESP_LOGI("TAG", "Distancia vazia: %.2f cm", empty_distance);
                    ssd1306_clear_screen(&dev, false);
                    sprintf(buffer, "%.2f cm", empty_distance);
                    set_calibration_text(3, true, "Recipente vazio:");
                    set_calibration_text(5, true, buffer);
                    vTaskDelay(pdMS_TO_TICKS(3000));
                    button_pressed = 0;
                    ssd1306_clear_screen(&dev, false);
                    set_calibration_text(1, true, "Medir distancia");
                    set_calibration_text(2, true, "Recipiente cheio:");
                    set_calibration_text(4, true, "Pressione o");
                    set_calibration_text(5, true, "botao novamente!");
                } else if (empty_distance != 0 && full_distance == 0) {
                    full_distance = get_distance();
                    ssd1306_clear_screen(&dev, false);
                    sprintf(buffer, "%.2f cm", full_distance);
                    set_calibration_text(3, true, "Recipiente cheio:");
                    set_calibration_text(5, true, buffer);
                    ESP_LOGI("TAG", "Distancia cheia: %.2f cm", full_distance);
                    vTaskDelay(pdMS_TO_TICKS(3000));
                    xTaskCreate(home_screen, "home_screen", 4096, NULL, 1, NULL);
                }
                vTaskDelay(pdMS_TO_TICKS(3000));  // Aguarde 3 segundos antes de pressionar o botão novamente  
            }

        }

        // Verificar se a distância está próxima à variável de controle
        if (get_distance() >= empty_distance && key_red == 0) {
            turn_on_led(LED_RED_PIN); // Acender o LED vermelho
            key_red = 1;
        } else if (get_distance() <= empty_distance && key_red == 1){
            turn_off_led(LED_RED_PIN); // Apagar o LED vermelho
            key_red = 0;
        }

        // Verificar se a distância está próxima à variável de controle
        if (get_distance() <= (full_distance + 1) && key_green == 0) {
            turn_on_led(LED_GREEN_PIN); // Acender o LED verde
            key_green = 1;
        } else if (get_distance() >= (full_distance + 1) && key_green == 1){
            turn_off_led(LED_GREEN_PIN); // Apagar o LED verde
            key_green = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Aguarde 100 milissegundos antes de verificar novamente o botão
    }
}

int get_button_pressed(){
    return button_pressed;
}

int get_percent_distance() {
    float tamanho_intervalo = empty_distance - full_distance;
    float tamanho_parte = tamanho_intervalo / 20;

    float distancia_atual = get_distance();
    int porcentagem = 0;

    if (distancia_atual < full_distance){
        return 100;
    }
    if (distancia_atual > empty_distance){
        return 0;
    }

    for (int i = 0; i < 20; i++) {
        float valor_minimo_i = full_distance + i * tamanho_parte;
        float valor_maximo_i = full_distance + (i + 1) * tamanho_parte;

        if (distancia_atual >= valor_minimo_i && distancia_atual < valor_maximo_i) {
            porcentagem = ((i + 1) * 5) - 100;
            break;
        }
    }

    vTaskDelay(pdMS_TO_TICKS(500));
    porcentagem = (porcentagem < 0) ? -porcentagem : porcentagem;
    return porcentagem;
}


void set_button_pressed(int valor){
    button_pressed = valor;
}