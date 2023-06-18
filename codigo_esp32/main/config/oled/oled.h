#ifndef OLED_H
#define OLED_H
void init_oled();
void set_calibration_text(int page, bool center, char nome_config[16]);
void home_screen(int page, bool center, char nome_config[16]);
#endif