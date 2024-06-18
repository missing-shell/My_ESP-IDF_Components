#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"
#include "mqtt_client.h"

#include "cJSON.h"
#include "aliyun_ota.h"
#include "aliyun_ota_json.h"

static const char *TAG = "ALIYUN_OTA";
extern const uint8_t server_root_crt_start[] asm("_binary_root_crt_start");
extern const uint8_t server_root_crt_end[] asm("_binary_root_crt_end");

#define OTA_URL_SIZE 256

/**
 * @brief Handles HTTP events during the Alibaba Cloud OTA upgrade process.
 *
 * Listens and responds to key events in HTTP communication such as connection, receiving data, completion, and errors,
 * used for tracking the network interaction of the OTA upgrade task.
 *
 * @param evt Pointer to the HTTP event structure containing detailed information about the current event.
 *
 * @return Returns ESP_OK indicating successful event handling.
 */
static esp_err_t aliyun_ota_http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
        break;
    }
    return ESP_OK;
}

/**
 * @brief Formats the Alibaba Cloud OTA upgrade URL by removing double quotes.
 *
 * This function removes double quotes from the Alibaba Cloud OTA upgrade URL to ensure it can be used correctly for upgrading.
 * The quotes are removed because they may be included during transmission or storage and are unnecessary for the URL's use.
 *
 * @param aliyun_ota_url Pointer to the OTA upgrade URL string that needs formatting.
 * @param aliyun_ota_url_len Length of the OTA upgrade URL string, excluding the trailing double quotes.
 */
static void aliyun_ota_url_format(char *aliyun_ota_url, uint16_t aliyun_ota_url_len)
{
    // The parsed URL contains double quotes "", but these must be absent in the array URL.
    for (int i = 0; i < aliyun_ota_url_len; i++)
    {
        aliyun_ota_url[i] = aliyun_ota_url[i + 1]; // Removes the leading double quotes "" from the array.
    }
    aliyun_ota_url[aliyun_ota_url_len - 2] = '\0'; // Removes the trailing double quotes "" from the array.
}

/**
 * @brief Initialize Alibaba Cloud OTA HTTPS upgrade process
 *
 * Initializes the HTTPS OTA upgrade configuration using the provided Alibaba Cloud OTA update URL
 * and HTTP event handler callback function, then attempts to download firmware updates from the specified URL.
 *
 * @param aliyun_ota_url Pointer to the OTA update URL which should not contain double quotes.
 * @param _http_event_handler HTTP event handler callback function for managing events during the upgrade process.
 *
 * @return esp_err_t Operation result. Refer to ESP-IDF documentation for specific error codes.
 */
static esp_err_t aliyun_ota_https_init(const char *aliyun_ota_url, http_event_handle_cb _http_event_handler)
{
    esp_http_client_config_t config = {
        .url = aliyun_ota_url, // Uses the provided OTA URL; note that the URL should not contain double quotes here.
        .cert_pem = (char *)server_root_crt_start,
        .event_handler = _http_event_handler,
        .keep_alive_enable = true,
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };
    printf("Attempting to download update from %s...\n", config.url);
    esp_err_t ret = esp_https_ota(&ota_config);

    return ret;
}
esp_err_t aliyun_ota_init(char *json_data)
{
    aliyun_ota_config_t ali_ota_config;
    esp_err_t ret = aliyun_ota_config_parse_json(json_data, &ali_ota_config);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "parse json error");
        return ret;
    }

    char aliyun_ota_url[OTA_URL_SIZE];
    int aliyun_ota_url_len = strlen(ali_ota_config.url);
    memcpy(aliyun_ota_url, ali_ota_config.url, aliyun_ota_url_len);
    aliyun_ota_url_format(aliyun_ota_url, aliyun_ota_url_len);

    ret = aliyun_ota_https_init(aliyun_ota_url, aliyun_ota_http_event_handler);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "aliyun_ota_https_init error");
        return ret;
    }

    return ret;
}

void aliyun_ota_report_inform(char *json_data, esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;

    cJSON *jsonPacket = create_aliyun_ota_json_packet_from_full_json(json_data);
    if (jsonPacket)
    {
        char *jsonStr = cJSON_Print(jsonPacket);

        int ota_inform_len = strlen(cJSON_Print(jsonPacket));

        char ota_inform_buf[512];
        memcpy(ota_inform_buf, cJSON_Print(jsonPacket), ota_inform_len); // 将JSON格式数据复制到数组中，将以数组的形式传递给发布函数
        ota_inform_buf[ota_inform_len] = '\0';
        ESP_LOGI(TAG, "\n%s\n", ota_inform_buf); // 打印数据是否正确

        int msg_id = esp_mqtt_client_publish(client, CONFIG_AliYun_PUBLISH_TOPIC_OTA_INFORM, ota_inform_buf, strlen(ota_inform_buf), 0, 0);
        ESP_LOGI(TAG, "sent publish ota inform successful, msg_id=%d", msg_id);

        free(jsonStr);            // 释放打印出的字符串的内存
        cJSON_Delete(jsonPacket); // 删除JSON对象，释放内存
    }
    else
    {
        ESP_LOGE(TAG, "Failed to create JSON packet.\n");
    }
}