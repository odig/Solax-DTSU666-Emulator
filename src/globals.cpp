#include "globals.h"


// ===================== Global Objects =====================

WiFiServer debugServer(23); // Telnet port
WiFiClient debugClient;


WiFiClient wifiClient;
PubSubClient mqttClient;

DTSU666 dtsu666 = DTSU666(1); // Modbus RTU client for DTSU666 meter

Victron victron; // Modbus TCP client


bool otaReported = false;
bool registersAdded = false;