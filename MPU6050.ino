
/*
 * Definições de alguns endereços mais comuns do MPU6050
 * os registros podem ser facilmente encontrados no mapa de registros do MPU6050
 */
const int MPU_ADDR =      0x68; // definição do endereço do sensor MPU6050 (0x68)
const int WHO_AM_I =      0x75; // registro de identificação do dispositivo
const int PWR_MGMT_1 =    0x6B; // registro de configuração do gerenciamento de energia
const int GYRO_CONFIG =   0x1B; // registro de configuração do giroscópio
const int ACCEL_CONFIG =  0x1C; // registro de configuração do acelerômetro
const int ACCEL_XOUT =    0x3B; // registro de leitura do eixo X do acelerômetro


double gyroXrate,gyroYrate,gyroZrate,
    gyroXangle,gyroYangle,gyroZangle,
    accelXangle,accelYangle,accelZangle,
    compXangle,compYangle,compZangle,
    dt,pitch,roll,yaw;
float alpha,tau;
uint32_t timer;


void initI2C() {
  Wire.begin(sda_pin, scl_pin);
}

void writeRegMPU(int reg, int val) {
  Wire.beginTransmission(MPU_ADDR);     // inicia comunicação com endereço do MPU6050
  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
  Wire.write(val);                      // escreve o valor no registro
  Wire.endTransmission(true);           // termina a transmissão
}

uint8_t readRegMPU(uint8_t reg) {
  uint8_t data;
  Wire.beginTransmission(MPU_ADDR);     // inicia comunicação com endereço do MPU6050
  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
  Wire.endTransmission(false);          // termina transmissão mas continua com I2C aberto (envia STOP e START)
  Wire.requestFrom(MPU_ADDR, 1);        // configura para receber 1 byte do registro escolhido acima
  data = Wire.read();                   // lê o byte e guarda em 'data'
  return data;                          //retorna 'data'
}

void findMPU(int mpu_addr) {
  Wire.beginTransmission(MPU_ADDR);
  int data = Wire.endTransmission(true);
  if(data == 0) {
    Serial.print("Dispositivo encontrado no endereço: 0x");
    Serial.println(MPU_ADDR, HEX);
  } else {
    Serial.println("Dispositivo não encontrado!");
  }
}

void checkMPU() {
  findMPU(MPU_ADDR);
  int data = readRegMPU(WHO_AM_I); // Register 117 – Who Am I - 0x75
  if(data == 104) {
    Serial.println("MPU6050 Dispositivo respondeu OK! (104)");
    data = readRegMPU(PWR_MGMT_1); // Register 107 – Power Management 1-0x6B
    if(data == 64) Serial.println("MPU6050 em modo SLEEP! (64)");
    else Serial.println("MPU6050 em modo ACTIVE!");
  }
  else Serial.println("Verifique dispositivo - MPU6050 NÃO disponível!");
}

void initMPU() {
  setSleepOff();
  setGyroScale();
  setAccelScale();
  compXangle=compYangle=compZangle=0;
  tau = 1.0;
  delay(2000);
}

void setSleepOff() {
  writeRegMPU(PWR_MGMT_1, 0); // escreve 0 no registro de gerenciamento de energia(0x68), colocando o sensor em o modo ACTIVE
}

/* função para configurar as escalas do giroscópio
   registro da escala do giroscópio: 0x1B[4:3]
   0 é 250°/s
    FS_SEL  Full Scale Range
      0        ± 250 °/s      0b00000000
      1        ± 500 °/s      0b00001000
      2        ± 1000 °/s     0b00010000
      3        ± 2000 °/s     0b00011000
*/
void setGyroScale() {
  writeRegMPU(GYRO_CONFIG, 0);
}

/* função para configurar as escalas do acelerômetro
   registro da escala do acelerômetro: 0x1C[4:3]
   0 é 250°/s
    AFS_SEL   Full Scale Range
      0           ± 2g            0b00000000
      1           ± 4g            0b00001000
      2           ± 8g            0b00010000
      3           ± 16g           0b00011000
*/
void setAccelScale() {
  writeRegMPU(ACCEL_CONFIG, 0);
}

void readRawMPU() {
  Wire.beginTransmission(MPU_ADDR);       // inicia comunicação com endereço do MPU6050
  Wire.write(ACCEL_XOUT);                 // envia o registro com o qual se deseja trabalhar, começando com registro 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);            // termina transmissão mas continua com I2C aberto (envia STOP e START)
  Wire.requestFrom(MPU_ADDR, 14);         // configura para receber 14 bytes começando do registro escolhido acima (0x3B)

  dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  AcX = Wire.read() << 8;                 // lê primeiro o byte mais significativo
  AcX |= Wire.read();                     // depois lê o bit menos significativo
  AcY = Wire.read() << 8;
  AcY |= Wire.read();
  AcZ = Wire.read() << 8;
  AcZ |= Wire.read();

  Tmp = Wire.read() << 8;
  Tmp |= Wire.read();
  Tmp = Tmp/340.00+36.53;

  GyX = Wire.read() << 8;
  GyX |= Wire.read();
  GyY = Wire.read() << 8;
  GyY |= Wire.read();
  GyZ = Wire.read() << 8;
  GyZ |= Wire.read();


 /*
  * Convert to deg/s, depende de la sensibilidad
Angular Velocity Limit  |   Sensitivity
----------------------------------------
250º/s                  |    131
500º/s                  |    65.5
1000º/s                 |    32.8
2000º/s                 |    16.4
  */
  gyroXrate = DEG_TO_RAD * dt * GyX / 131.0;
  gyroYrate = DEG_TO_RAD * dt * GyY / 131.0;
  gyroZrate = DEG_TO_RAD * dt * GyZ / 131.0;

  gyroXangle = compXangle - gyroXrate;
  gyroYangle = compYangle - gyroYrate;
  gyroZangle = compZangle - gyroZrate;

  Serial.print("gyro "); Serial.print(gyroXangle); Serial.print(" . "); Serial.print(gyroYangle); Serial.print(" . "); Serial.println(gyroZangle);

  accelXangle = -atan2(AcY, sqrt(AcX * AcX + AcZ * AcZ));
  accelYangle = atan2(AcX, sqrt(AcY * AcY + AcZ * AcZ));
  accelZangle = atan2(sqrt(AcY * AcY + AcY * AcY), AcZ);

  Serial.print("accel "); Serial.print(accelXangle); Serial.print(" . "); Serial.println(accelYangle);

  alpha = tau/(tau+dt);
  compXangle = alpha*gyroXangle + (1.0f - alpha)*accelXangle;
  compYangle = alpha*gyroYangle + (1.0f - alpha)*accelYangle;
  compZangle = alpha*gyroZangle + (1.0f - alpha)*accelZangle;

  pitch = compXangle;
  roll= compYangle;
  yaw = compZangle;

  led_state = !led_state;
  digitalWrite(LED_BUILTIN, led_state);         // pisca LED do NodeMCU a cada leitura do sensor
}
