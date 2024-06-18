#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "sdkconfig.h"

#include "cJSON.h"
#include "aliyun_ota_json.h"

static const char *TAG = "ALIYUN_OTA_JSON";
esp_err_t aliyun_ota_config_parse_json(const char *json_data, aliyun_ota_config_t *config)
{
    cJSON *root = cJSON_Parse(json_data);
    if (!root)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        return ESP_FAIL;
    }

    config->code = cJSON_Print(cJSON_GetObjectItem(root, "code"));
    config->size = cJSON_GetObjectItem(root, "data")->child->valueint;

    cJSON *Pdata = cJSON_GetObjectItem(root, "data");
    config->_package_udi = cJSON_Print(cJSON_GetObjectItem(Pdata, "_package_udi"));
    config->sign = cJSON_Print(cJSON_GetObjectItem(Pdata, "sign"));
    config->version = cJSON_Print(cJSON_GetObjectItem(Pdata, "version"));
    config->url = cJSON_Print(cJSON_GetObjectItem(Pdata, "url"));
    config->signMethod = cJSON_Print(cJSON_GetObjectItem(Pdata, "signMethod"));
    config->md5 = cJSON_Print(cJSON_GetObjectItem(Pdata, "md5"));

    config->id = cJSON_GetObjectItem(root, "id")->valuestring;
    config->message = cJSON_Print(cJSON_GetObjectItem(root, "message"));

    cJSON_Delete(root); // 删除root对象，释放其内存

    return ESP_OK;
}

void aliyun_ota_config_free(aliyun_ota_config_t *config)
{
    if (config)
    {
        free((void *)config->code);
        free((void *)config->_package_udi);
        free((void *)config->sign);
        free((void *)config->version);
        free((void *)config->url);
        free((void *)config->signMethod);
        free((void *)config->md5);
        free((void *)config->message);
    }
}

cJSON *create_aliyun_ota_json_packet_from_full_json(const char *json_data)
{
    if (json_data == NULL)
        return NULL;

    cJSON *root = cJSON_Parse(json_data);
    if (!root)
    {
        ESP_LOGE(TAG, "Error parsing JSON data: %s", cJSON_GetErrorPtr());
        return NULL;
    }

    const char *id_str = "123";
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    if (id_item && id_item->valuestring && strlen(id_item->valuestring) > 0)
    {
        id_str = id_item->valuestring;
    }

    const char *version = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "data"), "version")->valuestring;

    cJSON *new_root = cJSON_CreateObject();
    cJSON_AddItemToObject(new_root, "id", cJSON_CreateString(id_str));

    cJSON *params = cJSON_CreateObject();
    cJSON_AddItemToObject(params, "version", cJSON_CreateString(version));
    cJSON_AddItemToObject(new_root, "params", params);

    cJSON_Delete(root);

    return new_root;
}