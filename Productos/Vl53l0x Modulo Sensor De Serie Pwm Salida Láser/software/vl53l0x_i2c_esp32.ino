/*
 * VL53L0X (modulo de 4 pines) - Medicion de distancia por I2C
 * Placa: ESP32 (cualquiera)
 *
 * Conexion:
 *   VIN -> 3.3V   (no 5V: el chip es de 2.8V)
 *   GND -> GND
 *   SCL -> GPIO22 (ESP32 clasico)  o  GPIO6 (ESP32-C3 SuperMini)
 *   SDA -> GPIO21 (ESP32 clasico)  o  GPIO5 (ESP32-C3 SuperMini)
 *
 * Direccion I2C: 0x29 en 7 bits. El datasheet la escribe 0x52, que es la
 * de 8 bits (0x52 >> 1 = 0x29). Es el mismo sensor.
 *
 * Libreria: VL53L0X de Pololu (Library Manager -> buscar "VL53L0X")
 */

#include <Wire.h>
#include <VL53L0X.h>

// --- Pines I2C: descomenta el par que corresponda a tu placa ---
#define PIN_SDA 21      // ESP32 clasico (DevKit CH340C)
#define PIN_SCL 22

// #define PIN_SDA 5    // ESP32-C3 SuperMini: NO uses 8 y 9 (LED y boton BOOT)
// #define PIN_SCL 6

VL53L0X sensor;

void setup() {
  Serial.begin(115200);
  delay(500);

  Wire.begin(PIN_SDA, PIN_SCL);

  sensor.setTimeout(500);

  if (!sensor.init()) {
    Serial.println("No se detecta el VL53L0X.");
    Serial.println("Revisa: 3.3V entre VIN y GND, y que SDA/SCL no esten cruzados.");
    while (1) delay(1000);
  }

  Serial.println("VL53L0X listo.");

  // --- Perfil de medicion ---
  // Presupuesto de tiempo por lectura. Mas tiempo = mas precision, menos velocidad.
  //   33000 us = 33 ms  -> por defecto, uso general
  //  200000 us = 200 ms -> alta precision (+/-3 %)
  //   20000 us = 20 ms  -> rapido (+/-5 %)
  sensor.setMeasurementTimingBudget(33000);

  // Perfil LONG RANGE (hasta 2 m, solo a oscuras): descomenta estas tres lineas
  // sensor.setSignalRateLimit(0.1);
  // sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  // sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  // Modo continuo: el sensor mide solo y nosotros leemos cuando queremos
  sensor.startContinuous();
}

void loop() {
  uint16_t mm = sensor.readRangeContinuousMillimeters();

  if (sensor.timeoutOccurred()) {
    Serial.println("TIMEOUT - el sensor dejo de responder");
  } else if (mm >= 8190) {
    // 8190/8191 = "fuera de rango": nada que reflejar, objeto muy lejos,
    // superficie oscura, o demasiada luz solar
    Serial.println("Fuera de rango");
  } else {
    Serial.print(mm);
    Serial.print(" mm  (");
    Serial.print(mm / 10.0, 1);
    Serial.println(" cm)");
  }

  delay(100);
}
