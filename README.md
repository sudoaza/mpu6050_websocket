
Sketch para leer el MPU6050 desde el ESP8266.
Usa un filtro complementario para sacar el pitch y el roll. El yaw deriva feo.
Usa la libreria de MQTT de Adafruit para enviar los datos al broker de HiveMQ.
Incluye un HTML+JS para conectarse al broker por WebSocket (HiveMQ tiene soporte para WS) y mover un cubo en 3D (usando three.js).

## Instrucciones

* Conectar el SDA al D5, el SDL al D6 y los pines de VCC y GND a donde corresponde.
* Configurar el WiFi
* Flashar
* Abrir el HTML en el browser.

