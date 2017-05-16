
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  Serial.println("\nConfigurando MPU6050\n");
  initI2C();
  initMPU();
  checkMPU();
  readRawMPU();
}

void loop() {
  MQTT_connect();

  readRawMPU();
  json["roll"] = roll;
  json["pitch"] = pitch;
  json["yaw"] = yaw;

  if ( micros() - last_pub > 200000 ) {
    char buf[json.measureLength() + 1];
    json.printTo(buf, json.measureLength() + 1);
    
    Serial.print("\nSending val ");
    Serial.print(buf);
    Serial.print("...");
    
    last_pub = micros();
    
    if (! mqtt_chan.publish(buf)) {
      Serial.println("Failed");
    } else {
      Serial.println("OK!");
    }
  }

  delay(10);
}
