#include "MPU6050_Sensor.h"

MPU6050_Sensor::MPU6050_Sensor(std::string id, Gateway *gw, JsonObject *state_) : Sensor(id, gw, state_) {
  state->createNestedObject("model");
  (*state)["type"] = std::string("MPU");
  (*state)["model"] = std::string("MPU6050");
}
