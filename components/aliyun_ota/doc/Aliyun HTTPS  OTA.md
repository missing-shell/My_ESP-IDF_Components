# Aliyun_HTTPS_OTA

## 1. 概述

首先以Aliyun_MQTT_OTA作为基础工程。

**修改分区表**：确保flash size>=**4mb**

**选择分区模式**：可选官方推荐分区表，也可在项目根目录自建分区表。

## 2. 解析URL

使用Aliyun_MQTT_OTA和云端通信，解析云端下发的升级包

## 3. 代码实现

![Aliyun OTA 代码流程](./Aliyun%20HTTPS%20OTA代码流程.png)

关于代码流程时间序列图的原始UML代码[Aliyun OTA 代码流程.puml](./Aliyun%20HTTPS%20OTA%20代码流程.puml)
