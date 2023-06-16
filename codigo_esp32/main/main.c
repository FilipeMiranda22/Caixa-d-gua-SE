#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "config/temperatura/temperatura.h"
#include "config/oled/oled.h"

void app_main(void)
{
    init_oled();
    xTaskCreate(init_temperatura, "temperature_task", 2048, NULL, 5, NULL);
    ssd1306_display_text(&dev, 0, "Temperatura:", 12, false);
    ssd1306_display_text(&dev, 2, "Nivel:", 12, false);
}
