#include "esp_err.h"

esp_err_t wifi_init_sta();

/* Perform HTTP GET
Return content
Caller is responsible for freeing the returned memory
*/
char* http_get(char const* url);