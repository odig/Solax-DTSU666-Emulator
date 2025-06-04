#pragma once

void setupMQTT();
void handleMqtt();
void mqttCallback(char *topic, byte *payload, unsigned int length);
