#include "oled.h"
#include "network.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "mdns.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char const *TAG = "main";

void app_main() {
    // Initialize NVS // Non Volatile Storage => storage that persists when power goes off
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

    init_display();
    int l = 0;
    display_text(l++, "     /\\");
    display_text(l++, "     \\/");
    display_text(l++, " --(.)(.)--");
    display_text(l++, "     ##");
    display_text(l++, "     ##");
    display_text(l++, "     ##");
    display_text(l++, "    /  \\");
    display_text(l++, "   /    \\");

	// Initialize WiFi
	if (wifi_init_sta() != ESP_OK) {
		ESP_LOGE(TAG, "Connection failed");
        display_text(1, "Connection failed");
		while(1) { vTaskDelay(1); }
	}
    display_text(1, "WIFI: " CONFIG_ESP_WIFI_SSID);

	char *TARGET_HOST = "192.168.137.1";
	ESP_LOGI(TAG, "target host is %s", TARGET_HOST);

    char const* response = http_get("http://192.168.137.1:3000");
    ESP_LOGI(TAG, "Received response: %s", response);

    display_text(2, "Response:");
    display_text(3, response);
}
