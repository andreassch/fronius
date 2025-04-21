#include <Arduino.h>

enum class FroniusError
{
  ERR_NONE,
  ERR_WIFI,
  ERR_HTTP,
  ERR_DESERIALIZE,
};

struct fronius_t
{
  float p_pv = 0;
  float p_load = 0;
  float p_grid = 0;
  float p_accu = 0;
  float charge = 0;
  FroniusError error = FroniusError::ERR_NONE;
};

class FroniusInverter
{
public:
  void begin(const char* hostname);
  fronius_t query();

private:
  String m_hostname;
};
