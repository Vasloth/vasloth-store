/*
 * ESP32-C3 SuperMini - Test de placa
 *
 * Verifica que la placa este viva: parpadea el LED, imprime info del chip,
 * lee el ADC y escanea las redes WiFi.
 *
 * IMPORTANTE: esta placa NO tiene chip USB-serie. El puerto serie lo hace
 * el propio ESP32-C3 (USB CDC). Si no activas "USB CDC On Boot: Enabled"
 * en el menu Herramientas, NO vas a ver nada en el Monitor Serie.
 * Ver README.md.
 */

#include <WiFi.h>

#define LED_PIN 8        // LED azul de la placa (logica invertida: LOW = encendido)
#define PIN_ADC 0        // GPIO0 = ADC1_CH0

void setup() {
  Serial.begin(115200);

  // Con USB CDC el puerto tarda en levantar. Esperamos hasta 3 s a que el
  // monitor se conecte, pero no bloqueamos para siempre si corre sin PC.
  unsigned long t0 = millis();
  while (!Serial && millis() - t0 < 3000) {
    delay(10);
  }

  pinMode(LED_PIN, OUTPUT);

  Serial.println();
  Serial.println("=== ESP32-C3 SuperMini ===");
  Serial.print("Chip:      ");
  Serial.println(ESP.getChipModel());
  Serial.print("Revision:  ");
  Serial.println(ESP.getChipRevision());
  Serial.print("CPU:       ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");
  Serial.print("Flash:     ");
  Serial.print(ESP.getFlashChipSize() / (1024 * 1024));
  Serial.println(" MB");
  Serial.print("RAM libre: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  Serial.print("MAC:       ");
  Serial.println(WiFi.macAddress());

  // Escaneo WiFi: sirve para ver que tan mal anda la antena de esta placa
  Serial.println("\nEscaneando WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("  No se encontraron redes.");
  } else {
    Serial.print("  ");
    Serial.print(n);
    Serial.println(" redes:");
    for (int i = 0; i < n && i < 10; i++) {
      Serial.print("   ");
      Serial.print(WiFi.SSID(i));
      Serial.print("  ");
      Serial.print(WiFi.RSSI(i));
      Serial.println(" dBm");
    }
  }
  Serial.println();
}

void loop() {
  // LED invertido: LOW enciende
  digitalWrite(LED_PIN, LOW);
  delay(200);
  digitalWrite(LED_PIN, HIGH);
  delay(800);

  // ADC1_CH0 en GPIO0. Resolucion 12 bits -> 0..4095
  int lectura = analogRead(PIN_ADC);
  Serial.print("ADC GPIO0: ");
  Serial.print(lectura);
  Serial.print("  (");
  Serial.print(lectura * 3.3 / 4095.0, 2);
  Serial.println(" V aprox)");
}
