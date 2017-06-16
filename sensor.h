#ifndef __SENSOR_H_INCLUDED__
#define __SENSOR_H_INCLUDED__ 1

#include "thing.h"

class Sensor : Thing {
public:
    Sensor(std::string id, Gateway *gw, JsonObject *state_);
};

#endif
