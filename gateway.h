#ifndef __GATEWAY_H_INCLUDED__
#define __GATEWAY_H_INCLUDED__ 1

#include <ArduinoJson.h>
#include <string>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

class Gateway {
public:
  std::string id;
  Gateway(std::string _id, Client *net_client, Adafruit_MQTT *mqtt_client);
  void loop();
  std::string topic();

private:
  Adafruit_MQTT *mqtt;
  Client *net;
  void MQTTConnect();
};

#endif
