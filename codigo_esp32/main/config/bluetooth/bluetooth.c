#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"

#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/ble_hs_stop.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "sdkconfig.h"

#include "../temperatura/temperatura.h"

#define MAX_TEMP_LENGTH 32

char *TAG = "BLE-Server";
uint8_t ble_addr_type;
void ble_app_advertise(void);
char text;
uint8_t mac[6];
char mac_str[19];

static char user_bluetooth_temp[MAX_TEMP_LENGTH];     // Variável global para armazenar o user_bluetooth_temp

void disconnect_bluetooth() {
    esp_nimble_hci_and_controller_deinit();
    ble_hs_deinit();
    nimble_port_freertos_deinit();
}

char* get_mac_bt(){
    esp_read_mac(mac, ESP_MAC_BT);
    sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X\n", 
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return mac_str;
}

char* get_bluetooth_temp(){
    return user_bluetooth_temp;
}

void set_bluetooth_temp(const char* temp) {
    strncpy(user_bluetooth_temp, temp, MAX_TEMP_LENGTH - 1);
    user_bluetooth_temp[MAX_TEMP_LENGTH - 1] = '\0';
}

// Write data to ESP32 defined as server
static int user_bluetooth_bluetooth_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    ESP_LOGI("TAG", "User Temp Atualizado: %.*s", ctxt->om->om_len, ctxt->om->om_data);
    const char* data = (const char*) ctxt->om->om_data;
    strncpy(user_bluetooth_temp, data, ctxt->om->om_len);           // Copia o novo user_bluetooth_temp para a variável global
    user_bluetooth_temp[ctxt->om->om_len] = '\0';
    float value = atof(user_bluetooth_temp);
    set_temp_user(value);
    return 0;
}

// Read data from ESP32 defined as server
static int user_bluetooth_temp_read(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    ESP_LOGI("TAG", "user_bluetooth_temp: %s", user_bluetooth_temp);
    os_mbuf_append(ctxt->om, user_bluetooth_temp, strlen(user_bluetooth_temp));
    return 0;
}


// Array of pointers to other service definitions
// UUID - Universal Unique Identifier
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(0x180),                // Define UUID for device type
        .characteristics = (struct ble_gatt_chr_def[]){
        {
            .uuid = BLE_UUID16_DECLARE(0xF221),           // Define UUID for writing
            .flags = BLE_GATT_CHR_F_WRITE,
            .access_cb = user_bluetooth_bluetooth_write
        },
        {
            .uuid = BLE_UUID16_DECLARE(0xA221),           // Define UUID for reading
            .flags = BLE_GATT_CHR_F_READ,
            .access_cb = user_bluetooth_temp_read
        },
        {0}
        }
    },
    {0}
    
    };

// BLE event handling
static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {
    // Advertise if connected
    case BLE_GAP_EVENT_CONNECT:
        ESP_LOGI("GAP", "BLE GAP EVENT CONNECT %s", event->connect.status == 0 ? "OK!" : "FAILED!");
        if (event->connect.status != 0)
        {
            ble_app_advertise();
        }
        break;
    // Advertise again after completion of the event
    case BLE_GAP_EVENT_ADV_COMPLETE:
        ESP_LOGI("GAP", "BLE GAP EVENT");
        ble_app_advertise();
        break;
    case BLE_GAP_EVENT_DISCONNECT:
        break;
    default:
        break;
    }
    return 0;
}

// Define the BLE connection
void ble_app_advertise(void)
{
    // GAP - device name definition
    struct ble_hs_adv_fields fields;
    const char *device_name;
    memset(&fields, 0, sizeof(fields));
    device_name = ble_svc_gap_device_name(); // Read the BLE device name
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;
    ble_gap_adv_set_fields(&fields);

    // GAP - device connectivity definition
    struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND; // connectable or non-connectable
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN; // discoverable or non-discoverable
    ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
}

// The application
void ble_app_on_sync(void)
{
    ble_hs_id_infer_auto(0, &ble_addr_type); // Determines the best address type automatically
    ble_app_advertise();                     // Define the BLE connection
}

// The infinite task
void host_task(void *param)
{
    nimble_port_run(); // This function will return only when nimble_port_stop() is executed
}

void bluetooth_start()
{
    nvs_flash_init();                          // 1 - Initialize NVS flash using
    esp_nimble_hci_and_controller_init();      // 2 - Initialize ESP controller
    nimble_port_init();                        // 3 - Initialize the host stack
    ble_svc_gap_device_name_set("TEMPERATURA"); // 4 - Initialize NimBLE configuration - server name
    ble_svc_gap_init();                        // 4 - Initialize NimBLE configuration - gap service
    ble_svc_gatt_init();                       // 4 - Initialize NimBLE configuration - gatt service
    ble_gatts_count_cfg(gatt_svcs);            // 4 - Initialize NimBLE configuration - config gatt services
    ble_gatts_add_svcs(gatt_svcs);             // 4 - Initialize NimBLE configuration - queues gatt services.
    ble_hs_cfg.sync_cb = ble_app_on_sync;      // 5 - Initialize application
    nimble_port_freertos_init(host_task);      // 6 - Run the thread
}