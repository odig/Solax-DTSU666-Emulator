#include "globals.h"
#include "Arduino.h"

// ===================== Helper Functions =====================

void handleStatusReport()
{
  // Debug reporting
  static unsigned long lastReport = 0;
  if (millis() - lastReport > 5000)
  {
    dtsu666.printStatus();

    // Report OTA status
    if (enableOTA && !otaReported)
    {
      otaReported = true;
      dpf("[SYSTEM] OTA update service active on %s.local:%d\n",
              otaHostname, otaPort);
    }

    // debugClient ? dpf("[DEBUG] Client connected: %s:%d\n", debugClient.remoteIP().toString().c_str(), debugClient.remotePort())
    //             : dpf("[DEBUG] No client connected\n");

    lastReport = millis();
  }
}

// ===================== Setup Functions =====================

void setupWiFi()
{
  Serial.printf("[WiFi] Connecting to %s\n", ssid);

  WiFi.setHostname(wifiHostname);
  WiFi.begin(ssid, wifiPassword);

  delay(500);
  int i = 1;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (i > 1000)
    {
      Serial.printf("[WiFi] Failed to connect to WiFi");
      return;
    }
    Serial.printf("[WiFi] retry %d\n", i++);
    delay(500);
  }
  Serial.printf("[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
}


void setupSerial()
{
  if (enableSerialLogs)
  {
    Serial.begin(115200);
    delay(5000);
  }
}

// ===================== Main Program =====================

void setup()
{
  setupSerial();

  dpln("[Setup] Smart Meter Emulator Starting...\n");

  setupWiFi();

  if (enableOTA && WiFi.status() == WL_CONNECTED)
  {
    if (enableTelnetDebugging)
    {
      dpln("[DEBUG] Telnet debugging enabled");
      debugServer.begin();
      debugServer.setNoDelay(true);
    }

    setupOTA();
    setupMQTT();
    setupWebServer();
    victron.setupVictronModbusSlave();
  }
}

void loop()
{
  dtsu666.handle();
  victron.handleVictronModbusSlave();
  handleMqtt();

  handleTelnetDebugging();
  handleOta();
  handleWebServer();
  handleStatusReport();

  // Yield to system
  delay(1);
}
