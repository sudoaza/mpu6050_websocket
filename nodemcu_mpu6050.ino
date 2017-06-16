#include <ESP8266WiFi.h>
#include "MPU6050_Sensor.h"

const int I2C_SDA = D5;
const int I2C_SCL = D6;

#define WLAN_SSID "Compartir!"
#define WLAN_PASS "Compartir!"

WiFiClient wifi;

#define MQTT_SERVER      "broker.hivemq.com"
#define MQTT_SERVERPORT  1883 // use 8883 for SSL
#define MQTT_USERNAME    ""
#define MQTT_KEY ""

Gateway *gateway;
const int JSON_BUFF_SIZE = 1024;
StaticJsonBuffer<JSON_BUFF_SIZE> jsonBuffer;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Adafruit_MQTT_Client mqtt(&wifi, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);
  Gateway *gateway = new Gateway(std::string("ESP8266AZA"), &wifi, &mqtt);
  JsonObject& state = jsonBuffer.createObject();
  MPU6050_Sensor acelerometro = new MPU6050_Sensor(std::string("MPU6050AZA"), &gateway, &state);
  acelerometro.initI2C(I2C_SDA, I2C_SCL);
  
}

void loop() {
  gateway->loop();
}
