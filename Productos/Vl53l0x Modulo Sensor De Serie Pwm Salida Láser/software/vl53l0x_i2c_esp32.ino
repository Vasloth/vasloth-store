/*
 * GY-53 (VL53L0X) - Medicion de distancia por I2C
 *
 * ALTERNATIVA al modo serie. El GY-53 arranca en modo serie y ese es el
 * camino recomendado: usa gy53_uart_esp32.ino, que no necesita libreria.
 *
 * Para usar I2C hay que poner el pin PS (pin 6) A GND. Asi el STM32 del
 * modulo se desentiende y le hablamos al chip VL53L0X directo.
 * En este modo la salida PWM deja de funcionar.
 *
 * Direccion I2C: 0x29 en 7 bits (el datasheet la escribe 0x52, que es la de
 * 8 bits; 0x52 >> 1 = 0x29). Es el mismo sensor.
 *
 * Libreria: VL53L0X de Pololu (ver la guia de software)
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
    Serial.println("Revisa: alimentacion 3.3V, SDA/SCL, y en el GY-53 que PS este a GND.");
    while (1) delay(1000);
  }

  Serial.println("VL53L0X listo.");

  // Presupuesto de tiempo: cuanto mas alto, mas preciso y mas lento.
  // 33000 us = 33 ms (por defecto).  200000 us = 200 ms (alta precision).
  sensor.setMeasurementTimingBudget(33000);

  // Modo continuo: el sensor mide solo y nosotros leemos cuando queremos
  sensor.startContinuous();
}

void loop() {
  uint16_t mm = sensor.readRangeContinuousMillimeters();

  if (sensor.timeoutOccurred()) {
    Serial.println("TIMEOUT - el sensor dejo de responder");
  } else if (mm >= 8190) {
    // Valores de 8190/8191 significan "fuera de rango": nada que reflejar,
    // objeto muy lejos, superficie oscura, o demasiada luz solar
    Serial.println("Fuera de rango");
  } else {
    Serial.print(mm);
    Serial.print(" mm  (");
    Serial.print(mm / 10.0, 1);
    Serial.println(" cm)");
  }

  delay(100);
}
