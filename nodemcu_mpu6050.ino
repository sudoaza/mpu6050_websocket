#include <ESP8266WiFi.h>  // biblioteca para usar as funções de Wifi do módulo ESP8266
#include <Wire.h>         // I2C Para comunicarse con el MPU
#include <ArduinoJson.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// pines I2C
const int sda_pin = D5;
const int scl_pin = D6;

bool led_state = false;

// variáveis para armazenar os dados "crus" do acelerômetro
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; 

#define WLAN_SSID "Compartir!"
#define WLAN_PASS "Compartir!"

#define MQTT_SERVER      "broker.hivemq.com"
#define MQTT_SERVERPORT  1883                   // use 8883 for SSL
#define MQTT_USERNAME    ""
#define MQTT_KEY ""


// construindo o objeto JSON que irá armazenar os dados do acelerômetro na função populateJSON()
StaticJsonBuffer<300> jsonBuffer;
JsonObject& json = jsonBuffer.createObject();
JsonObject& j_roll = json.createNestedObject("roll");
JsonObject& j_pitch = json.createNestedObject("pitch");
JsonObject& j_yaw = json.createNestedObject("yaw");

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);
Adafruit_MQTT_Publish mqtt_chan = Adafruit_MQTT_Publish(&mqtt, "/thisismychan");

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
uint32_t last_pub;

