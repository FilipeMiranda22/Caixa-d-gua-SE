#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "ds18x20.h"
#include "../oled/oled.h"

#define DS18B20_GPIO GPIO_NUM_3

void init_temperatura(){
    ds18x20_addr_t addrs[1];
    SSD1306_t dev;
    float temps[1];
    size_t sensor_count;
    char buffer[50];

    //Find all sensors on the bus
    ESP_ERROR_CHECK(ds18x20_scan_devices(DS18B20_GPIO, addrs, 1, &sensor_count));

    printf("Found %d sensors\n", sensor_count);

    while (1) {
        // Start temperature conversion on all sensors
        ESP_ERROR_CHECK(ds18x20_measure_and_read_multi(DS18B20_GPIO, addrs, sensor_count, temps));

        // Print results
        for (int i = 0; i < sensor_count; i++) {
            //sprintf(buffer, "Sensor %d: %.1f°C\n", i, temperatura);
            printf("Sensor %d: %.1f°C\n", i, temps[i]);
            //ssd1306_display_text(&dev, 2, buffer, 12, false);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}