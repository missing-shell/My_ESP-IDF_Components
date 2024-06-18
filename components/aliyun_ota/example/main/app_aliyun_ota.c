#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "mqtt_client.h"
#include "aliyun_mqtt.h"
#include "aliyun_ota.h"
#include "aliyun_ota_json.h"

char local_data_buffer[1024] = {0};
char mqtt_publish_data1[] = "mqtt connect ok ";
char mqtt_publish_data2[] = "mqtt subscribe successful";
char mqtt_publish_data3[] = "mqtt i am esp32";

static const char *TAG = "APP_ALIYUN_OTA";
static void app_aliyun_ota_start(esp_mqtt_event_handle_t event);
static esp_err_t app_mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{

    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_publish(client, CONFIG_AliYun_PUBLISH_TOPIC_USER_UPDATE, mqtt_publish_data1, strlen(mqtt_publish_data1), 1, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, CONFIG_AliYun_SUBSCRIBE_TOPIC_USER_GET, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        // 注意：这里使用了同样的发布主题，根据实际逻辑可能需要调整
        msg_id = esp_mqtt_client_publish(client, CONFIG_AliYun_PUBLISH_TOPIC_USER_UPDATE, mqtt_publish_data2, strlen(mqtt_publish_data2), 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);

        break;
    case MQTT_EVENT_DATA:

        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        app_aliyun_ota_start(event);

        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}
static void app_mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%ld", base, event_id);
    app_mqtt_event_handler_cb(event_data);
}

static void app_aliyun_ota_start(esp_mqtt_event_handle_t event)
{
    char *topicBuf = (char *)malloc(event->topic_len + 1);
    // void  *memcpy(void *s1,  const void *s2,  size_t  n); 函数memcpy从s2指向的对象中复制n个字符到s1指向的对象中。
    memcpy(topicBuf, event->topic, event->topic_len);

    int temp = strcmp(topicBuf, CONFIG_AliYun_SUBSCRIBE_TOPIC_OTA_UPGRADE); // 判断是否是OTA主题

    if (temp == 0)
    {
        ESP_LOGD(TAG, "OTA DATA PARSE");
        // char  *strncpy(char *s2, const char *s1, size_t n);函数strncpy从s1指向的数组中最多复制n个字符（不复制空字符后面的字符）到s2指向的数组中。
        strncpy(local_data_buffer, event->data, event->data_len); // 将指针类型的数据复制到一个数组中

        esp_err_t ret = aliyun_ota_init(local_data_buffer);
        if (ret == ESP_OK) // 解析并下载OTA升级包成功
        {
            ESP_LOGI(TAG, "解析并下载OTA升级包成功");
            aliyun_ota_report_inform(local_data_buffer, event);
            esp_restart();
        }
    }
}
void app_aliyun_ota_init(void)
{
    /*MQTT*/
    aliyun_mqtt_init(app_mqtt_event_handler);
    /*OTA*/
}