// #include "main_wifi.c"

// #include "main_oled.c"

#include "wifi.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "mdns.h"
#include "ping.h"

static const char *TAG = "main";

void app_main() {
    // Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	// Initialize WiFi
	if (wifi_init_sta() != ESP_OK) {
		ESP_LOGE(TAG, "Connection failed");
		while(1) { vTaskDelay(1); }
	}

	char *TARGET_HOST = "www.espressif.com";
	ESP_LOGI(TAG, "target host is www.espressif.com");

	if (initialize_ping(1000, 2, TARGET_HOST) == ESP_OK) {
		ESP_LOGI(TAG, "initialize_ping success");
	} else {
		ESP_LOGE(TAG, "initialize_ping fail");
	}
}
