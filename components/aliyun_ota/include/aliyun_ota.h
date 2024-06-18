#ifndef _ALIYUN_OTA_H_
#define _ALIYUN_OTA_H_

#include "esp_http_client.h"
#include "mqtt_client.h"
#include "esp_log.h"

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * @brief Initializes the Alibaba Cloud OTA process by parsing configuration, formatting the URL, and setting up HTTPS.
     *
     * This function parses the OTA configuration from JSON data, formats the OTA URL to remove unnecessary characters,
     * and initializes the HTTPS connection for the Alibaba Cloud OTA upgrade process.
     *
     * @param json_data Pointer to the JSON formatted configuration data for OTA.
     * @return esp_err_t Result of the operation, ESP_OK on success.
     */
    esp_err_t aliyun_ota_init(char *json_data);
    /**
     * @brief Reports OTA inform message via MQTT after processing provided JSON data.
     *
     * Parses the full JSON data into a compact JSON packet, prints it, and then publishes the message
     * to the MQTT topic configured for Alibaba Cloud OTA inform messages.
     *
     * @param json_data Pointer to the full JSON data to be processed for the inform message.
     * @param event MQTT event handle used to identify the MQTT client for publishing.
     */
    void aliyun_ota_report_inform(char *json_data, esp_mqtt_event_handle_t event);
#ifdef __cplusplus
}
#endif

#endif