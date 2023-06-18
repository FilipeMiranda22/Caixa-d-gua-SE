#ifndef LED_H
#define LED_H
#define LED_RED_PIN GPIO_NUM_4
#define LED_GREEN_PIN GPIO_NUM_5
void init_led();
void turn_off_led(int gpio);
void turn_on_led(int gpio);
#endif