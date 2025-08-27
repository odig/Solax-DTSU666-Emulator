#pragma once

// Web server functions

// ===================== Configuration =====================
// Features
extern const bool enableWebServer;       // Enable web server for configuration and monitoring
extern const bool enableModbus;          // Enable Modbus RTU communication
extern const bool enableMQTT;            // Enable MQTT communication
extern const bool enableOTA;             // Enable OTA updates for remote management
extern const bool enableTelnetDebugging; // Enable debugging features

// Debugging
extern const bool enableSerialLogs;         // Set to true for debug output
extern const bool logModbusRegisterActions; // Log Modbus register actions
extern const bool logMqttUpdates;           // Log MQTT updates
extern const bool enableSerialHttpLogs;     // Enable serial logging for HTTP requests

// WiFi Credentials
extern const char *ssid;         // WiFi SSID
extern const char *wifiPassword; // WiFi password
extern const char *wifiHostname; // Hostname for the device on the network

// Victron modbusTCP connection
extern IPAddress gxIP;   // Replace with your GX device's IP

// MQTT Configuration
extern const char *mqttBroker;   // MQTT broker address
extern const int mqttPort;       // MQTT broker port
extern const char *mqttClientID; // MQTT client ID
extern const char *mqttUsername; // MQTT username
extern const char *mqttPassword; // MQTT username and password
extern const char *mqttTopic;    // MQTT topic for data
extern const char *statusTopic;  // MQTT topic for status updates

// Modbus Configuration
extern const long modbusBaudRate; // Modbus baud rate
extern const int RX_PIN;          // RX pin for Modbus communication
extern const int TX_PIN;          // TX pin for Modbus communication
extern const int SERIAL_CONFIG;   // Serial configuration (8N1)

// OTA Update Configuration
extern const char *otaHostname;
extern const char *otaPassword; // Strong password for OTA updates
extern const int otaPort;       // Default port for ESP OTA

#define TOPIC_ENERGY_FORWARD "victron/N/48e7da88e3bb/grid/30/Ac/Energy/Forward"
#define TOPIC_ENERGY_REVERSE "victron/N/48e7da88e3bb/grid/30/Ac/Energy/Reverse"
#define TOPIC_POWER "victron/N/48e7da88e3bb/grid/30/Ac/Power"
#define TOPIC_L1_VOLTAGE "victron/N/48e7da88e3bb/grid/30/Ac/L1/Voltage"
#define TOPIC_L2_VOLTAGE "victron/N/48e7da88e3bb/grid/30/Ac/L2/Voltage"
#define TOPIC_L3_VOLTAGE "victron/N/48e7da88e3bb/grid/30/Ac/L3/Voltage"
#define TOPIC_L1_CURRENT "victron/N/48e7da88e3bb/grid/30/Ac/L1/Current"
#define TOPIC_L2_CURRENT "victron/N/48e7da88e3bb/grid/30/Ac/L2/Current"
#define TOPIC_L3_CURRENT "victron/N/48e7da88e3bb/grid/30/Ac/L3/Current"
#define TOPIC_L1_POWER "victron/N/48e7da88e3bb/grid/30/Ac/L1/Power"
#define TOPIC_L2_POWER "victron/N/48e7da88e3bb/grid/30/Ac/L2/Power"
#define TOPIC_L3_POWER "victron/N/48e7da88e3bb/grid/30/Ac/L3/Power"
#define TOPIC_FREQUENCY "victron/N/48e7da88e3bb/grid/30/Ac/Frequency"
