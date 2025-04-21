#include "fronius.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

void FroniusInverter::begin(const char* hostname)
{
  m_hostname = String(hostname);
}

fronius_t FroniusInverter::query()
{
  fronius_t result;
  HTTPClient http;
  String url = "http://" + m_hostname + "/solar_api/v1/GetPowerFlowRealtimeData.fcgi";
  http.begin(url);
  int http_code = http.GET();
  if(http_code > 0)
  {
    if(http_code == HTTP_CODE_OK)
    {
      String payload = http.getString();
      DynamicJsonDocument doc(900);
      DeserializationError error = deserializeJson(doc, payload);
      if (error)
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        result.error = FroniusError::ERR_DESERIALIZE;
      }
      else
      {
        auto node = doc["Body"]["Data"]["Site"];
        result.p_pv = node["P_PV"].as<float>();
        result.p_load = node["P_Load"].as<float>();
        result.p_grid = node["P_Grid"].as<float>();
        result.p_accu = node["P_Akku"].as<float>();
        result.charge = doc["Body"]["Data"]["Inverters"]["1"]["SOC"].as<float>();
        result.error = FroniusError::ERR_NONE;
      }
    }
    else
    {
      result.error = FroniusError::ERR_HTTP;
    }
    http.end();
  }
  else
  {
    result.error = FroniusError::ERR_WIFI;
  }
  return result;
}
