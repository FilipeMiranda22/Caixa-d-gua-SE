idf_component_register(SRCS "main.c"
                    "config/temperatura/temperatura.c"
                    "config/oled/oled.c"
                    "config/distancia/distancia.c"
                    "config/botao/botao.c"
                    "config/led/led.c"
                    "config/atuador/atuador.c"
                    INCLUDE_DIRS "."
                    REQUIRES "ultrasonic" "esp_idf_lib_helpers" "nvs_flash" "ds18x20" "ssd1306")
