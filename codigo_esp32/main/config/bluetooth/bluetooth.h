#ifndef BLUETOOTH_H
#define BLUETOOTH_H
void bluetooth_start();
char* get_mac_bt();
void disconnect_bluetooth();
char* get_bluetooth_temp();
void set_bluetooth_temp(const char* temp);
#endif