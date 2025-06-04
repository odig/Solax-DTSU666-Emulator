#include "globals.h"



WebServer server(80);

String htmlPage()
{
  String page = "<!DOCTYPE html><html><head><meta charset='utf-8'><title>ESP32 Smart Meter</title>";
  page += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  page += "<style>body{font-family:sans-serif;}table{border-collapse:collapse;}td,th{border:1px solid #ccc;padding:6px;}th{background:#eee;}</style>";
  page += "<script>function update(){fetch('/data').then(r=>r.json()).then(d=>{";
  page += "for(let k in d){let e=document.getElementById(k);if(e)e.innerText=d[k];}";
  page += "});}setInterval(update,2000);window.onload=update;</script>";
  page += "</head><body>";
  page += "<h2>ESP32 Smart Meter</h2>";
  page += "<table>";
  page += "<tr><th>Field</th><th>Value</th><th>Status</th></tr>";
  page += "<tr><td>Total kWh</td><td id='import'></td><td id='import_rx'></td></tr>";
  page += "<tr><td>Export kWh</td><td id='export'></td><td id='export_rx'></td></tr>";
  page += "<tr><td>Total Power (W)</td><td id='total_power'></td><td id='total_power_rx'></td></tr>";
  page += "<tr><td>Voltage L1 (V)</td><td id='v1'></td><td id='v1_rx'></td></tr>";
  page += "<tr><td>Voltage L2 (V)</td><td id='v2'></td><td id='v2_rx'></td></tr>";
  page += "<tr><td>Voltage L3 (V)</td><td id='v3'></td><td id='v3_rx'></td></tr>";
  page += "<tr><td>Current L1 (A)</td><td id='c1'></td><td id='c1_rx'></td></tr>";
  page += "<tr><td>Current L2 (A)</td><td id='c2'></td><td id='c2_rx'></td></tr>";
  page += "<tr><td>Current L3 (A)</td><td id='c3'></td><td id='c3_rx'></td></tr>";
  page += "<tr><td>Power L1 (W)</td><td id='power_a'></td><td id='power_a_rx'></td></tr>";
  page += "<tr><td>Power L2 (W)</td><td id='power_b'></td><td id='power_b_rx'></td></tr>";
  page += "<tr><td>Power L3 (W)</td><td id='power_c'></td><td id='power_c_rx'></td></tr>";
  page += "<tr><td>Frequency (Hz)</td><td id='freq'></td><td id='freq_rx'></td></tr>";
  page += "</table>";
  page += "<h3>Modbus Status</h3>";
  page += "<div id='modbus'></div>";
  page += "<script>function modbusStat(){fetch('/modbus').then(r=>r.text()).then(t=>{document.getElementById('modbus').innerText=t;});}setInterval(modbusStat,2000);window.onload=modbusStat;</script>";
  page += "</body></html>";
  return page;
}

void handleRoot()
{
  server.send(200, "text/html", htmlPage());
}

void handleData()
{
  String json = "{";
  json += "\"import\":" + String(meterData.total_import) + ",";
  json += "\"import_rx\":\"" + String(meterData.total_import_rx ? "OK" : "WAIT") + "\",";

  json += "\"export\":" + String(meterData.total_export) + ",";
  json += "\"export_rx\":\"" + String(meterData.total_import_rx ? "OK" : "WAIT") + "\",";

  json += "\"total_power\":" + String(meterData.total_power) + ",";
  json += "\"total_power_rx\":\"" + String(meterData.total_power_rx ? "OK" : "WAIT") + "\",";
  
  json += "\"v1\":" + String(meterData.v1) + ",";
  json += "\"v1_rx\":\"" + String(meterData.v1_rx ? "OK" : "WAIT") + "\",";
  json += "\"v2\":" + String(meterData.v2) + ",";
  json += "\"v2_rx\":\"" + String(meterData.v2_rx ? "OK" : "WAIT") + "\",";
  json += "\"v3\":" + String(meterData.v3) + ",";
  json += "\"v3_rx\":\"" + String(meterData.v3_rx ? "OK" : "WAIT") + "\",";

  json += "\"c1\":" + String(meterData.c1) + ",";
  json += "\"c1_rx\":\"" + String(meterData.c1_rx ? "OK" : "WAIT") + "\",";
  json += "\"c2\":" + String(meterData.c2) + ",";
  json += "\"c2_rx\":\"" + String(meterData.c2_rx ? "OK" : "WAIT") + "\",";
  json += "\"c3\":" + String(meterData.c3) + ",";
  json += "\"c3_rx\":\"" + String(meterData.c3_rx ? "OK" : "WAIT") + "\",";

  json += "\"power_a\":" + String(meterData.power_1) + ",";
  json += "\"power_a_rx\":\"" + String(meterData.power_1_rx ? "OK" : "WAIT") + "\",";
  json += "\"power_b\":" + String(meterData.power_2) + ",";
  json += "\"power_b_rx\":\"" + String(meterData.power_2_rx ? "OK" : "WAIT") + "\",";
  json += "\"power_c\":" + String(meterData.power_3) + ",";
  json += "\"power_c_rx\":\"" + String(meterData.power_3_rx ? "OK" : "WAIT") + "\",";

  json += "\"freq\":" + String(meterData.freq) + ",";
  json += "\"freq_rx\":\"" + String(meterData.freq_rx ? "OK" : "WAIT") + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

void handleModbus()
{
  String stat = modbusInitialized ? "Modbus server running" : "Waiting for all data fields";
  server.send(200, "text/plain", stat);
}

void setupWebServer()
{
  if (!enableWebServer)
  {
    return;
  }  

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/modbus", handleModbus);
  server.begin();
  if (enableSerialHttpLogs)
    Serial.println("[Web] Web server started on port 80");
}

void handleWebServer()
{
  if (!enableWebServer)
  {
    return;
  }  

  server.handleClient();
}
