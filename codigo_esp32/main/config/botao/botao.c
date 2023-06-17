#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#define BUTTON_PIN GPIO_NUM_0

int button_pressed = 0;

void button_task() {
    gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    while (1) {
        if (gpio_get_level(BUTTON_PIN) == 0 && !button_pressed) {
            // Botão pressionado
            ESP_LOGI("BOTAO", "APERTEI");
            button_pressed = 1;
            vTaskDelay(pdMS_TO_TICKS(3000));  // Aguarde 3 segundos antes de pressionar o botão novamente
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Aguarde 100 milissegundos antes de verificar novamente o botão
    }
}

int get_button_pressed(){
    return button_pressed;
}

void set_button_pressed(int valor){
    button_pressed = valor;
}