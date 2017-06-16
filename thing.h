#ifndef __THING_H_INCLUDED__
#define __THING_H_INCLUDED__ 1

#include <ArduinoJson.h>
#include "gateway.h"

class Thing {
public:
  JsonObject * state;
  Gateway *gateway;
  uint32_t last_pub = 0;

  Thing(std::string id, Gateway *gw, JsonObject *state_);
  virtual std::string topic();
};

#endif
