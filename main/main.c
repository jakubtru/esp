// #include "main_wifi.c"

// #include "main_oled.c"
#include "oled.h"

void app_main(void) {
    dotstuff();
}


// #include "wifi.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include "nvs_flash.h"
// #include "mdns.h"
// #include "ping.h"
// #include "esp_system.h"
// #include "esp_http_client.h"

// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

// static const char *TAG = "main";

// esp_err_t http_event_handler(esp_http_client_event_t *evt)
// {
//     switch(evt->event_id) {
//         case HTTP_EVENT_ERROR:
//             ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
//             break;
//         case HTTP_EVENT_ON_CONNECTED:
//             ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
//             break;
//         case HTTP_EVENT_HEADER_SENT:
//             ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
//             break;
//         case HTTP_EVENT_ON_HEADER:
//             ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER");
//             printf("%.*s", evt->data_len, (char*)evt->data);
//             break;
//         case HTTP_EVENT_ON_DATA:
//             ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
//             if (!esp_http_client_is_chunked_response(evt->client)) {
//                 printf("%.*s", evt->data_len, (char*)evt->data);
//             }
//             break;
//         case HTTP_EVENT_ON_FINISH:
//             ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
//             break;
//         case HTTP_EVENT_DISCONNECTED:
//             ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
//             break;
//         default:
//             ESP_LOGW(TAG, "another event. event_id: %i", evt->event_id);
//             break;
//     }
//     return ESP_OK;
// }

// #define WEB_URL "http://192.168.137.1:3000"

// static void http_rest_with_url(void)
// {
//     esp_http_client_config_t config = {
//         .url = WEB_URL,
//         .event_handler = http_event_handler,
//     };
//     esp_http_client_handle_t client = esp_http_client_init(&config);

//     // Performing the request
//     esp_err_t err = esp_http_client_perform(client);

//     if (err == ESP_OK) {
//         ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %lld",
//                  esp_http_client_get_status_code(client),
//                  esp_http_client_get_content_length(client));
//     } else {
//         ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
//     }

//     esp_http_client_cleanup(client);
// }

// void app_main() {
//     // Initialize NVS // Non Volatile Storage => storage that persists when power goes off
// 	esp_err_t ret = nvs_flash_init();
// 	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
// 		ESP_ERROR_CHECK(nvs_flash_erase());
// 		ret = nvs_flash_init();
// 	}
// 	ESP_ERROR_CHECK(ret);

// 	// Initialize WiFi
// 	if (wifi_init_sta() != ESP_OK) {
// 		ESP_LOGE(TAG, "Connection failed");
// 		while(1) { vTaskDelay(1); }
// 	}

// 	char *TARGET_HOST = "192.168.137.1";
// 	ESP_LOGI(TAG, "target host is %s", TARGET_HOST);

//     http_rest_with_url();

// // 	if (initialize_ping(1000, 2, TARGET_HOST) == ESP_OK) {
// // 		ESP_LOGI(TAG, "initialize_ping success");
// // 	} else {
// // 		ESP_LOGE(TAG, "initialize_ping fail");
// // 	}
// }
