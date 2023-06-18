#include "driver/gpio.h"
#include "esp_log.h"

#define LED_RED_PIN GPIO_NUM_5
#define LED_GREEN_PIN GPIO_NUM_4

void init_led(){
    gpio_pad_select_gpio(LED_RED_PIN);
    gpio_set_direction(LED_RED_PIN, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(LED_GREEN_PIN);
    gpio_set_direction(LED_GREEN_PIN, GPIO_MODE_OUTPUT);
}

void turn_on_led(int gpio){
    
        ESP_LOGI("TAG", "ASCENDI LED %d", gpio);
        gpio_set_level(gpio, 1); // Acender o LED vermelho
    
}

void turn_off_led(int gpio){
    
        ESP_LOGI("TAG", "APAGUEI LED %d", gpio);
        gpio_set_level(gpio, 0); // Desligar o LED vermelho
    
}