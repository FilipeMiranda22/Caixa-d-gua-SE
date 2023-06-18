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

void init_oled(void)
{
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
}

void set_calibration_text(int page, bool center, char nome_config[16]){
    if (!center){
        ssd1306_display_text(&dev, page, nome_config, strlen(nome_config), false);
    } else {
        ssd1306_display_text_centered(&dev, page, nome_config, strlen(nome_config), false);
    }
}

void home_screen(int page, bool center, char nome_config[16]){
    ssd1306_clear_screen(&dev, false);
    while (1)
    {   
        ssd1306_display_text_centered(&dev, 0, "HOME", 16, false);
        sprintf(bufferTemp, "Temp: %.1f C", get_temp());
        ssd1306_display_text(&dev, 3, bufferTemp, 16, false);
        sprintf(buffer, "Nivel: %d%%", get_percent_distance());
        ssd1306_display_text(&dev, 5, buffer, 12, false);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
}

//void temp_screen(int page, bool)