#include "driver/gpio.h"

void turn_on_led(int gpio){
    gpio_set_level(gpio, 1); // Acender o LED vermelho
}

void turn_off_led(int gpio){
    gpio_set_level(gpio, 0); // Desligar o LED vermelho
}