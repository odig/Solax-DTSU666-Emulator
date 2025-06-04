#include "globals.h"

void setupOTA()
{
  if (!enableOTA)
    return;

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname(otaHostname);

  // No authentication by default
  // ArduinoOTA.setPassword(otaPassword);

  ArduinoOTA.onStart([]()
                     { dpln("Start OTA update"); });

  ArduinoOTA.onEnd([]()
                   { dpln("[OTA] Update complete"); });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { dpf("[OTA] Progress: %u%%\r", (progress / (total / 100))); });

  ArduinoOTA.onError([](ota_error_t error)
                     {
      dpf("[OTA] Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) dpln("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) dpln("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) dpln("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) dpln("Receive Failed");
      else if (error == OTA_END_ERROR) dpln("End Failed"); });

  ArduinoOTA.begin();

  dpln("[OTA] OTA service initialized");
}

void handleOta()
{
  // Handle OTA updates
  if (enableOTA)
  {
    ArduinoOTA.handle();
  }
}