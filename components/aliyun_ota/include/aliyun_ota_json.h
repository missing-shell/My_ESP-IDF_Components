#ifndef _ALIYUN_OTA_JSON_H_
#define _ALIYUN_OTA_JSON_H_

#include "cJSON.h"
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Structure to hold Alibaba Cloud OTA configuration details.
     *
     * This structure encapsulates various parameters necessary for an Alibaba Cloud OTA update,
     * including code, package size, unique device identifier (_package_udi), cryptographic sign,
     * version number, download URL, signing method, MD5 checksum, ID, and a message field.
     */
    typedef struct
    {
        char *code;         /**< Identifier for the OTA update code. */
        int size;           /**< Size of the OTA update package in bytes. */
        char *_package_udi; /**< Unique Device Identifier for the target device. */
        char *sign;         /**< Cryptographic signature of the OTA package. */
        char *version;      /**< Version string of the OTA package. */
        char *url;          /**< Download URL for the OTA package. */
        char *signMethod;   /**< Method used for signing the OTA package. */
        char *md5;          /**< MD5 checksum of the OTA package. */
        char *id;           /**< Identifier for the update request or message. */
        char *message;      /**< Additional informational message related to the update. */
    } aliyun_ota_config_t;

    /**
     * @brief Parses JSON data to populate an aliyun_ota_config_t structure.
     *
     * Extracts OTA configuration details from a JSON string and fills the corresponding fields in the provided config structure.
     *
     * @param json_data Pointer to the JSON string containing the OTA configuration.
     * @param config Pointer to an aliyun_ota_config_t structure to be populated with the parsed data.
     * @return esp_err_t Error code indicating the result of the operation.
     */
    esp_err_t aliyun_ota_config_parse_json(const char *json_data, aliyun_ota_config_t *config);

    /**
     * @brief Frees memory allocated within an aliyun_ota_config_t structure.
     *
     * Releases memory allocated for strings within the aliyun_ota_config_t structure to prevent memory leaks.
     *
     * @param config Pointer to the aliyun_ota_config_t structure whose memory should be freed.
     */
    void aliyun_ota_config_free(aliyun_ota_config_t *config);

    /**
     * @brief Creates a compact JSON packet from full JSON data for OTA purposes.
     *
     * Parses a complete JSON string and constructs a smaller, more focused JSON object (packet) that likely includes
     * only the necessary fields for an OTA-related message or request.
     *
     * @param json_data Pointer to the original, full JSON string.
     * @return cJSON* Pointer to the newly created JSON object representing the compact packet, or NULL on failure.
     */
    cJSON *create_aliyun_ota_json_packet_from_full_json(const char *json_data);
#ifdef __cplusplus
}
#endif

#endif