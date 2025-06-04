#include "globals.h"


// ===================== Global Objects =====================

WiFiServer debugServer(23); // Telnet port
WiFiClient debugClient;

WiFiClient wifiClient;
PubSubClient mqttClient;

ModbusRTU modbus;
ModbusIP victron; // Modbus TCP client

MeterData meterData;

bool otaReported = false;
bool modbusInitialized = false;
bool registersAdded = false;