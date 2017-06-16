#if !defined(MPU6050SENSOR)
#define MPU6050SENSOR 1

#include "sensor.h"
#include "thing.h"

class MPU6050_Sensor : Sensor {
public:
  MPU6050_Sensor(std::string id, Gateway *gw, JsonObject *state_);

  void initI2C(int sda, int scl);
};

#endif
