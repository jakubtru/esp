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
#include "mqtt_client.h"
// ADDED
//  #include "esp_task_wdt.h"

// std libs
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char const *TAG = "main";

void init();
void wifi_sanity_check();

// my changes
static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0)
    {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

bool should_stop_mqtt = false;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

        // Publish message to the topic
        msg_id = esp_mqtt_client_publish(client, "sensor/2", "Initial hello after connecting", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        // Delay for 2sec (adjust the duration as needed)
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        // Subscribe to the topic
        msg_id = esp_mqtt_client_subscribe(client, "sensor/2", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);

        // Publish message after subscribing
        msg_id = esp_mqtt_client_publish(client, "sensor/2", "Message after subscribing", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish after subscribing successful, msg_id=%d", msg_id);

        // Delay for 10sec (adjust the duration as needed)
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        msg_id = esp_mqtt_client_unsubscribe(client, "sensor/2");
        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);

        // Disconnect after unsubscribing
        
        should_stop_mqtt = true;
        break;

    case MQTT_EVENT_UNSUBSCRIBED:
        // Delay for 5sec (adjust the duration as needed)
        // vTaskDelay(5000 / portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;

    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

esp_mqtt_client_handle_t client = NULL;
static void mqtt_app_start(void)
{
    static const char *mqtt_username = "sensor1";
    static const char *mqtt_password = "test";

    ESP_LOGI(TAG, "STARTING MQTT");
    const esp_mqtt_client_config_t mqtt_cfg = {
        //.broker.address.uri = "mqtt://mqtt.eclipseprojects.io:1883",
        .broker.address.uri = "mqtt://192.168.39.9",
        .credentials.username = mqtt_username,
        .credentials.authentication.password = mqtt_password
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}


void app_main()
{
    init();
    wifi_sanity_check();

    int prevWifi = true;
    http_rq_rs http_ping = {
        .url = "init",
        .state = IDLE};

    mqtt_app_start();

    while (1)
    {
        if (should_stop_mqtt) //- nie działa nie wiem jeszcze czemu
        {
            ESP_LOGI(TAG, "Stopping MQTT client");
            esp_mqtt_client_disconnect(client);
            esp_mqtt_client_stop(client);
            should_stop_mqtt = false; // Reset the flag
        }

        if (http_ping.state == FINISHED)
        {
            display_text(2, http_ping.rs_data);
            free(http_ping.rs_data);
            http_ping.state = IDLE;
        }

        if (button_just_pressed())
        {
            ESP_LOGI(TAG, "Button pressed");
            if (is_wifi_connected() && http_ping.state == IDLE)
            {
                http_ping.url = "http://iot-server.glitch.me/ping";
                xTaskCreate((void (*)(void *))task_http_get, "Trying tasks", 4096, &http_ping, 10, NULL);
            }
        }

        if (!is_wifi_connected() && prevWifi)
        {
            display_text(0, "DISCONNECTED");
        }
        else if (is_wifi_connected() && !prevWifi)
        {
            display_text(0, "WIFI: " CONFIG_ESP_WIFI_SSID);
        }
        prevWifi = is_wifi_connected();

        vTaskDelay(pdMS_TO_TICKS(25));
    }
}

void init()
{
    // Non Volatile Storage => storage that persists when power goes off
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

    init_display();
    init_button();

    display_text(0, "Connecting...");
	if (wifi_init_sta() != ESP_OK) {
		ESP_LOGE(TAG, "Connection failed");
        display_text(0, "Connection failed");
		while(1) { vTaskDelay(1000); }
	}
    display_text(0, "WIFI: " CONFIG_ESP_WIFI_SSID);
}

void wifi_sanity_check() {
    const char* SANITY_URL = "http://example.com";
    ESP_LOGI(TAG, "Performing WIFI sanity check with %s", SANITY_URL);
    char* response = http_get(SANITY_URL);
    ESP_LOGI(TAG, "Received response: %s", response);
    display_text(1, "Response:");
    display_text(2, response);
    free(response);
}