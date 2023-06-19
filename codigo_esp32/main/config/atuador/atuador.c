#include "driver/gpio.h"

#define RESISTENCIA_GPIO GPIO_NUM_9
#define BOMBA_GPIO GPIO_NUM_8

void init_atuador(){
    gpio_pad_select_gpio(RESISTENCIA_GPIO);
    gpio_set_direction(RESISTENCIA_GPIO, GPIO_MODE_INPUT_OUTPUT);
    gpio_pad_select_gpio(BOMBA_GPIO);
    gpio_set_direction(BOMBA_GPIO, GPIO_MODE_OUTPUT);
}

void ativar_bomba(){
    gpio_set_level(BOMBA_GPIO, 1);
}

void desativar_bomba(){
    gpio_set_level(BOMBA_GPIO, 0);
}

void ativar_resistencia(){
    gpio_set_level(RESISTENCIA_GPIO, 1);
}

void desativar_resistencia(){
    gpio_set_level(RESISTENCIA_GPIO, 0);
}

int isResistenciaAtivada(){
    return gpio_get_level(RESISTENCIA_GPIO);
}

int isBombaAtivada(){
    return gpio_get_level(BOMBA_GPIO);
}