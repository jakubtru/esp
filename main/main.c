// custom logic
#include "oled.h"
#include "network.h"
#include "button.h"
// esp libs
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "mdns.h"
// std libs
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char const *TAG = "main";

void mainloop(void);

void app_main()
{
    // Initialize NVS // Non Volatile Storage => storage that persists when power goes off
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

    init_display();

    init_button();

    display_text(0, "Connecting...");

	// Initialize WiFi
	if (wifi_init_sta() != ESP_OK) {
		ESP_LOGE(TAG, "Connection failed");
        display_text(0, "Connection failed");
		while(1) { vTaskDelay(1); }
	}
    display_text(0, "WIFI: " CONFIG_ESP_WIFI_SSID);

    char const* response = http_get("http://192.168.137.1:3000/ping");
    ESP_LOGI(TAG, "Received response: %s", response);
    display_text(1, "Response:");
    display_text(2, response);


    while(1) {
        char buffer[20];
        sprintf(buffer, "Button: %d", button_pressed());
        display_text(3, buffer);

        if (!is_wifi_connected()) {
            esp_restart();
        }

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}