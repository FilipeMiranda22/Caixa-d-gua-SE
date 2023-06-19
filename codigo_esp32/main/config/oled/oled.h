#ifndef OLED_H
#define OLED_H
void init_oled();
void set_calibration_text(int page, bool center, char nome_config[16], bool clear_line);
void screen();
void details_screen();
void home_screen();
void set_number_screen(int valor);
int get_number_screen();
#endif