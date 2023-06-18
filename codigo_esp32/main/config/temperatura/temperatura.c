#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "ds18x20.h"
#include "../oled/oled.h"
#include "../botao/botao.h"
#include "ssd1306.h"
#include "esp_log.h"

float userTemp = 30;
float temp = 0;
#include "../atuador/atuador.h"

SSD1306_t dev;

void init_temperatura(){
    
    ds18x20_addr_t addrs[1];
    
    float temps[1];
    size_t sensor_count = 1;
    char buffer[50];

    //Find all sensors on the bus
    //ESP_ERROR_CHECK(ds18x20_scan_devices(DS18B20_GPIO, addrs, 1, &sensor_count));

    while (1) {
        // Start temperature conversion on all sensors
        //ESP_ERROR_CHECK(ds18x20_measure_and_read_multi(DS18B20_GPIO, addrs, sensor_count, temps));

        // Print results
        // for (int i = 0; i < sensor_count; i++) {
        for (int i = 15; i < 50; i++) {
            temp = i;
            //temp = temps[i];
            if ((temp < userTemp) && (get_percent_distance() > 10) && (!isResistenciaAtivada())){
                ativar_resistencia();
            }
            else if (isResistenciaAtivada() && ((get_percent_distance() <= 10) || (temp >= userTemp))){
                desativar_resistencia();
            }

            ESP_LOGI("TAG", "Sensor %d: %.1f°C\n", i, temp);

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        
    }
}

float get_temp(){
    return temp;
}