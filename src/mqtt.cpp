#include "globals.h"

// ===================== Helper Macro =====================

#define UPDATE_FIELD(field, var, flag)           \
  if (sml[#field].is<JsonVariant>())             \
  {                                              \
    dtsu666.meterData.var = sml[#field];  \
    if (!dtsu666.meterData.flag)          \
      dtsu666.meterData.flag = true;      \
    updated = true;                              \
                                                 \
    {                                            \
      dpf("- %s: ", #field);                     \
      dpf("%d\n", dtsu666.meterData.var); \
    }                                            \
  }

bool handleMqttTopic(char *topic, String topic_const, float value, float *field, bool *field_rx, const char *format, const char *unit)
{
  bool updated = false;
  if (!strcmp(topic, topic_const.c_str()))
  {
    *field = value;
    *field_rx = true;
    if (logMqttUpdates)
    {
      String formatStr = "- " + String(format) + String(unit) + "\n";
      dpf(formatStr.c_str(), *field);
    }
    updated = true;
  }
  return updated;
}

// ===================== MQTT Callback =====================
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  bool updated = false;

  //
  // {
  //   dpf("[MQTT] Message received on topic: %s\n", topic);
  //   dpf("[MQTT] Payload length: %d bytes\n", length);
  // }

  JsonDocument doc;

  // dump payload for debugging as ascii
  // if (enableSerialLogs)
  // {
  //   dpf("[MQTT] Payload:\n");
  //   dpf("-------------------\n");
  //   for (unsigned int i = 0; i < length; i++)
  //   {
  //     dpf("%c", payload[i]);
  //   }
  //   dpf("\n");
  //   dpf("-------------------\n");
  // }

  DeserializationError err = deserializeJson(doc, payload, length);
  if (err)
  {
    dpf("[MQTT] JSON error: %s\n", err.c_str());
    return;
  }

  // dump doc
  // if (enableSerialLogs)
  // {
  //   String jsonString;
  //   dpf("[MQTT] JSON Document:\n");
  //   dpf("*******************\n");
  //   serializeJson(doc, jsonString);
  //   dpf("%s\n",jsonString.c_str());
  //   dpf("*******************\n");
  // }

  if (!strcmp(topic, mqttTopic))
  {
    // handle json struct API
    if (!doc["SML"].is<JsonObject>())
    {

      dpln("[MQTT] Missing 'SML' object!");
      return;
    }

    JsonObject sml = doc["SML"];

    dpln("[MQTT] Updated fields:");
    UPDATE_FIELD(import_kw, total_import, total_import_rx)
    UPDATE_FIELD(export_kw, total_export, total_export_rx)
    UPDATE_FIELD(power_curr, total_power, total_power_rx)
    UPDATE_FIELD(volt_p1, v1, v1_rx)
    UPDATE_FIELD(volt_p2, v2, v2_rx)
    UPDATE_FIELD(volt_p3, v3, v3_rx)
    UPDATE_FIELD(current_p1, c1, c1_rx)
    UPDATE_FIELD(current_p2, c2, c2_rx)
    UPDATE_FIELD(current_p3, c3, c3_rx)
    UPDATE_FIELD(freq, freq, freq_rx)
    UPDATE_FIELD(power_p1, power_1, power_1_rx)
    UPDATE_FIELD(power_p2, power_2, power_2_rx)
    UPDATE_FIELD(power_p3, power_3, power_3_rx)
  }
  else
  {
    // Handle individual MQTT Victron topics
    if (!doc["value"].is<JsonVariant>())
    {
      dpln("[MQTT] Missing 'value' object!");
      return;
    }
    JsonVariant value = doc["value"];

    // Handle specific MQTT topics
    handleMqttTopic(topic, TOPIC_L1_VOLTAGE, value, &dtsu666.meterData.v1, &dtsu666.meterData.v1_rx, "L1 Voltage: %6.2f", "V");
    handleMqttTopic(topic, TOPIC_L2_VOLTAGE, value, &dtsu666.meterData.v2, &dtsu666.meterData.v2_rx, "L2 Voltage: %6.2f", "V");
    handleMqttTopic(topic, TOPIC_L3_VOLTAGE, value, &dtsu666.meterData.v3, &dtsu666.meterData.v3_rx, "L3 Voltage: %6.2f", "V");
    handleMqttTopic(topic, TOPIC_L1_CURRENT, value, &dtsu666.meterData.c1, &dtsu666.meterData.c1_rx, "L1 Current: %6.2f", "A");
    handleMqttTopic(topic, TOPIC_L2_CURRENT, value, &dtsu666.meterData.c2, &dtsu666.meterData.c2_rx, "L2 Current: %6.2f", "A");
    handleMqttTopic(topic, TOPIC_L3_CURRENT, value, &dtsu666.meterData.c3, &dtsu666.meterData.c3_rx, "L3 Current: %6.2f", "A");
    handleMqttTopic(topic, TOPIC_L1_POWER, value, &dtsu666.meterData.power_1, &dtsu666.meterData.power_1_rx, "L1 Power: %6.2f", "W");
    handleMqttTopic(topic, TOPIC_L2_POWER, value, &dtsu666.meterData.power_2, &dtsu666.meterData.power_2_rx, "L2 Power: %6.2f", "W");
    handleMqttTopic(topic, TOPIC_L3_POWER, value, &dtsu666.meterData.power_3, &dtsu666.meterData.power_3_rx, "L3 Power: %6.2f", "W");
    handleMqttTopic(topic, TOPIC_FREQUENCY, value, &dtsu666.meterData.freq, &dtsu666.meterData.freq_rx, "Frequency: %6.2f", "Hz");
    handleMqttTopic(topic, TOPIC_ENERGY_FORWARD, value, &dtsu666.meterData.total_import, &dtsu666.meterData.total_import_rx, "Energy Forward: %6.2f", "kWh");
    handleMqttTopic(topic, TOPIC_ENERGY_REVERSE, value, &dtsu666.meterData.total_export, &dtsu666.meterData.total_export_rx, "Energy Reverse: %6.2f", "kWh");
    handleMqttTopic(topic, TOPIC_POWER, value, &dtsu666.meterData.total_power, &dtsu666.meterData.total_power_rx, "Total Power: %6.2f", "W");
  }

  dtsu666.update();
}

bool connectMQTT()
{
  if (mqttClient.connected())
    return true;

  dpf("[MQTT] Connecting to %s...", mqttBroker);

  bool connected = mqttClient.connect(mqttClientID, mqttUsername, mqttPassword,
                                      statusTopic, 1, true, "offline");
  if (connected)
  {
    {
      dpln("[MQTT] connected!");
      dpf("[MQTT] Subscribed to: %s\n", mqttTopic);
    }
    mqttClient.publish(statusTopic, "online", true);
    mqttClient.subscribe(mqttTopic);

    mqttClient.subscribe(TOPIC_ENERGY_FORWARD);
    mqttClient.subscribe(TOPIC_ENERGY_REVERSE);
    mqttClient.subscribe(TOPIC_POWER);
    mqttClient.subscribe(TOPIC_L1_VOLTAGE);
    mqttClient.subscribe(TOPIC_L2_VOLTAGE);
    mqttClient.subscribe(TOPIC_L3_VOLTAGE);
    mqttClient.subscribe(TOPIC_L1_CURRENT);
    mqttClient.subscribe(TOPIC_L2_CURRENT);
    mqttClient.subscribe(TOPIC_L3_CURRENT);
    mqttClient.subscribe(TOPIC_L1_POWER);
    mqttClient.subscribe(TOPIC_L2_POWER);
    mqttClient.subscribe(TOPIC_L3_POWER);
    mqttClient.subscribe(TOPIC_FREQUENCY);
  }
  else
  {
    dpf("failed! State: %d\n", mqttClient.state());
  }
  return connected;
}

void handleMqtt()
{
  static unsigned long lastReconnectAttempt = 0;

  if (enableMQTT)
  {
    // Non-blocking MQTT management
    if (!mqttClient.connected())
    {
      if (millis() - lastReconnectAttempt > 5000)
      {
        lastReconnectAttempt = millis();
        connectMQTT();
      }
    }
    else
    {
      mqttClient.loop();
    }
  }
}

void setupMQTT()
{
  if (enableMQTT)
  {
    mqttClient.setClient(wifiClient);
    mqttClient.setServer(mqttBroker, mqttPort);
    mqttClient.setCallback(mqttCallback);
  }
}
