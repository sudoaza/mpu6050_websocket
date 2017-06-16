#include "thing.h"

class Thing {
public:

  Thing(std::string id, Gateway *gw, JsonObject *state_) : state(state_), gateway(gw) {
    state->createNestedObject("id");
    state->createNestedObject("type");
    (*state)["id"] = id;
  }

  virtual std::string topic() {
    return gateway->topic() + TOPIC_SEP + (*state)["id"].asString() + TOPIC_SEP + (*state)["type"].asString();
  }
};
