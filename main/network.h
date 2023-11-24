#include "esp_err.h"
#include <stdbool.h>

esp_err_t wifi_init_sta();

/*
Performs HTTP GET
Returns content
Caller is responsible for freeing the returned memory
*/
char* http_get(char const* url);

typedef struct {
    char* url;
    char* rs_data;
    bool finished;
} http_rq_rs;
// User is responsible of freeing arg
void task_http_get(http_rq_rs* arg);

// Making this a function so underlying bool is inaccessible outside network.c
bool is_wifi_connected();