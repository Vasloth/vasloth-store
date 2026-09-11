/*
 * DS18B20 sonda sumergible - Lectura de temperatura por 1-Wire
 * Placa: ESP32 (cualquiera) / Arduino
 *
 * Conexion (cables de la sonda):
 *   ROJO     (VDD) -> 3.3V
 *   AMARILLO (DQ)  -> GPIO 4  + resistencia 4.7k entre AMARILLO y ROJO
 *   NEGRO    (GND) -> GND
 *
 * La resistencia de 4.7k es OBLIGATORIA y NO viene con la sonda.
 * Sin ella vas a leer -127 C.
 *
 * OJO: los colores no son estandar y varian por lote. Hay sondas con
 * azul en vez de negro, o blanco en vez de amarillo. Verifica con
 * multimetro antes de alimentar: invertir VDD y GND puede quemarla.
 *
 * Librerias: OneWire + DallasTemperature (ver la guia de software)
 */

#include <OneWire.h>
#include <DallasTemperature.h>

#define PIN_ONEWIRE 4          // cambiar segun tu cableado

OneWire oneWire(PIN_ONEWIRE);
DallasTemperature sensores(&oneWire);

int cantidad = 0;

void buscarSensores();

void setup() {
  Serial.begin(115200);
  delay(500);

  sensores.begin();
  buscarSensores();
}

// Busca los sensores del bus y lista el ROM de cada uno.
// Se puede llamar las veces que haga falta.
void buscarSensores() {
  cantidad = sensores.getDeviceCount();

  Serial.print("Sensores encontrados: ");
  Serial.println(cantidad);

  if (cantidad == 0) {
    Serial.println("Ninguno. Revisa el pull-up de 4.7k y el cableado.");
    return;
  }

  // Imprime el ROM de 64 bits de cada sensor (util para identificarlos)
  for (int i = 0; i < cantidad; i++) {
    DeviceAddress rom;
    if (sensores.getAddress(rom, i)) {
      Serial.print("  #");
      Serial.print(i);
      Serial.print(" ROM: ");
      for (uint8_t b = 0; b < 8; b++) {
        if (rom[b] < 16) Serial.print("0");
        Serial.print(rom[b], HEX);
      }
      // El byte 0 es el codigo de familia: 0x28 = DS18B20 legitimo
      Serial.println(rom[0] == 0x28 ? "  (familia 0x28 OK)" : "  (familia RARA)");
    }
  }

  sensores.setResolution(12);   // 12 bits = 0.0625 C, conversion de 750 ms
}

void loop() {
  // Si no hay ninguno, reintenta cada 2 s: asi lo detecta aunque lo
  // enchufes despues de encender la placa.
  if (cantidad == 0) {
    delay(2000);
    sensores.begin();
    buscarSensores();
    return;
  }

  sensores.requestTemperatures();   // dispara la conversion y espera

  for (int i = 0; i < cantidad; i++) {
    float t = sensores.getTempCByIndex(i);

    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");

    // DEVICE_DISCONNECTED_C vale -127.0
    if (t == DEVICE_DISCONNECTED_C) {
      Serial.println("SIN RESPUESTA (-127) -> cableado o pull-up");
    } else if (t == 85.0) {
      // 85.0 exacto es el valor de reset del registro: no llego a convertir
      Serial.println("85.00 C sospechoso -> alimentacion floja o lectura prematura");
    } else {
      Serial.print(t, 2);
      Serial.println(" C");
    }
  }

  Serial.println("---");
  delay(2000);
}
