#include "sensor.h"

class Sensor {
public:
  Sensor(std::string id, Gateway *gw, JsonObject *state_) : Thing(id, gw, state_) {

  }
}
