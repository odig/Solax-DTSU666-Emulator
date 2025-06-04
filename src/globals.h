#pragma once

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <ModbusIP_ESP8266.h>
#include <ModbusRTU.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "dprint.h"      // Debug print functions
#include "modbus.h"      // Modbus handling functions
#include "mqtt.h"        // MQTT handling functions
#include "ota.h"         // OTA update handling functions
#include "settings.h"    // Configuration header
#include "smart_meter.h" // Smart meter data structure and functions
#include "victron.h"     // Victron Modbus handling functions
#include "web.h"         // Web server handling functions

// ===================== Global Objects =====================


extern WiFiServer debugServer; // Telnet port
extern WiFiClient debugClient;

extern WiFiClient wifiClient;
extern PubSubClient mqttClient;

extern ModbusRTU modbus;
extern ModbusIP victron; // Modbus TCP client

extern MeterData meterData;

extern bool otaReported;
extern bool modbusInitialized;
extern bool registersAdded;