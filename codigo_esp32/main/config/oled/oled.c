#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"


#include "ssd1306.h"
#include "font8x8_basic.h"

#include "config/botao/botao.h"
#include "config/temperatura/temperatura.h"


SSD1306_t dev;

char buffer[20]; 
char bufferTemp[20]; 
char bufferEmpty[20];
char bufferFull[20];
char bufferUserTemp[20];
int number_screen = 0;
int check = 0;

void init_oled(void)
{
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
}

void set_calibration_text(int page, bool center, char nome_config[16], bool clear_line){
    if (!center){
        if (clear_line){
            ssd1306_clear_line(&dev, page, false);
        } 
        ssd1306_display_text(&dev, page, nome_config, strlen(nome_config), false);
    } else {
        if (clear_line){
            ssd1306_clear_line(&dev, page, false);
        } 
        ssd1306_display_text_centered(&dev, page, nome_config, strlen(nome_config), false);
    }
}

void set_number_screen(int valor){
    number_screen = valor;
}

int get_number_screen(){
    return number_screen;
}

void home_screen(){
    ssd1306_clear_screen(&dev, false);
    while (number_screen == 0)
    {   
        set_calibration_text(0, 1, "HOME", false);
        sprintf(bufferTemp, "Temp: %.1f C", get_temp());
        ssd1306_display_text(&dev, 3, bufferTemp, 16, false);
        sprintf(buffer, "Nivel: %d%%", get_percent_distance());
        ssd1306_display_text(&dev, 5, buffer, 12, false);
        vTaskDelay(pdMS_TO_TICKS(300));
        
    }
}

void details_screen() {
    ssd1306_clear_screen(&dev, false);
    while (number_screen == 1)
    {
        set_calibration_text(0, 1, "DETAILS", false);
        sprintf(bufferFull, "Cheio: %.2f cm", get_full_distance());
        ssd1306_display_text(&dev, 3, bufferFull, 16, false);
        sprintf(bufferEmpty, "Vazio: %.2f cm", get_empty_distance());
        ssd1306_display_text(&dev, 5, bufferEmpty, 16, false);
        sprintf(bufferUserTemp, "Usr Temp: %.1f C", get_user_temp());
        ssd1306_display_text(&dev, 7, bufferUserTemp, 16, false);
        vTaskDelay(pdMS_TO_TICKS(300));
    }   
}

void screen(){
    while (1)
    { 
        if (number_screen == 0 && check == 0){
            check = 1;
            home_screen();
        }
        else if (number_screen == 1 && check == 1) {
            check = 0;
            details_screen();
        }
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}