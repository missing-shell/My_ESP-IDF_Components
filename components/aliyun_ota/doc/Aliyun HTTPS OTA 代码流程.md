# Aliyun HTTPS OTA 代码流程

## 初始化阶段

### 开始

- 应用程序主循环（`App`）启动时，首先调用`app_aliyun_ota_init`函数进行初始化。
  - 初始化MQTT客户端，并注册`app_mqtt_event_handler`监听MQTT事件。

## 事件响应

### MQTT事件处理

- 收到`MQTT_EVENT_DATA`事件后：
  - MQTT客户端通知`App`，激活`app_mqtt_event_handler`。
  - 事件处理器内检查事件类型，并决定调用`app_aliyun_ota_start`。

## OTA流程启动

### app_aliyun_ota_start

- 调用`aliyun_ota_init`开始OTA初始化。
  - 调用`aliyun_ota_config_parse_json`解析JSON获取OTA配置信息（如URL）。
  - 使用`aliyun_ota_url_format`格式化URL。
  - 根据解析结果，调用`aliyun_ota_https_init`初始化HTTPS连接。
    - **HTTPS初始化**
      - 注册`_aliyun_ota_http_event_handler`事件处理器。
      - 调用`esp_https_ota`启动HTTPS OTA更新。

## 执行OTA更新

### ESP组件接手

- **ESP HTTPS OTA组件**开始执行固件下载与更新。
- 更新完成后，控制权返回至`aliyun_ota_https_init`，进而返回至`aliyun_ota_init`。

## OTA完成

- 在`aliyun_ota_init`内部，调用`aliyun_ota_info_init`准备版本信息上报。
- 流程继续回到`app_aliyun_ota_start`，调用`aliyun_ota_report_inform`上报下载成功及新版本信息。

## 设备重启

- 最后，调用`esp_restart()`重启设备，使新固件生效。

### 流程总结

整个流程从应用初始化开始，通过MQTT事件触发OTA更新，历经OTA初始化、HTTPS配置、固件下载更新、状态上报，最终以设备自动重启结束，构成一个闭环的OTA升级流程。
