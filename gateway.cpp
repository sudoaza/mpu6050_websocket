#include "gateway.h"
#include <string>

const std::string TOPIC_SEP = "/";

Gateway::Gateway(std::string _id, Client *net_client, Adafruit_MQTT *mqtt_client) {
  id = _id;
  net = net_client;
  mqtt = mqtt_client;
}

void Gateway::loop() { MQTTConnect(); }

std::string Gateway::topic() { return id; }

void Gateway::MQTTConnect() {
  // Stop if already connected.
  if (mqtt->connected()) { return; }

  Serial.print("Connecting to MQTT... ");

  uint8_t ret, retries = 3;
  while ((ret = mqtt->connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt->connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt->disconnect();
    delay(3000);
    retries--;
    // basically die and wait for WDT to reset me
    if (retries == 0) { while (1); }
  }
  Serial.println("MQTT Connected!");
}
