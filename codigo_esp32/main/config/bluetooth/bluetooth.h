#ifndef BLUETOOTH_H
#define BLUETOOTH_H
void bluetooth_start();
int ready_to_send_wifi_data();
char* get_ssid();
char* get_password();
char* get_mac_bt();
void set_ssid(const char* ssid);
void set_password(const char* password);
void disconnect_bluetooth();
#endif