#include <WiFi.h>
#include <fronius.h>
#include "mywifi.h"
#include <type_traits>

FroniusInverter fronius;

void setup()
{
  Serial.begin(115200);
  for (int i = 0; !Serial && i < 50; i++)
  {
    delay(100);
  }

  // Connect to WiFi
  Serial.println(F("Connecting to "));
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }

  randomSeed(micros());

  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  fronius.begin(INVERTER_HOST);
}

void loop()
{
  fronius_t data = fronius.query();
  if (data.error == FroniusError::ERR_NONE)
  {
    Serial.print(F("P_PV: "));
    Serial.print(data.p_pv);
    Serial.print(F("W, P_Load: "));
    Serial.print(data.p_load);
    Serial.print(F("W, P_Grid: "));
    Serial.print(data.p_grid);
    Serial.print(F("W, P_Accu: "));
    Serial.print(data.p_accu);
    Serial.print(F("W, Charge: "));
    Serial.print(data.charge);
    Serial.println(F("%"));
  }
  else
  {
    Serial.print(F("Error querying Fronius inverter:  "));
    Serial.println(static_cast<std::underlying_type<FroniusError>::type>(data.error));
  }
  delay(1000);
}
