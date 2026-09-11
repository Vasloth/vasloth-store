/*
 * ESP32 USB-C CH340C - Test de placa
 *
 * Verifica que la placa este viva: parpadea el LED, imprime info del chip
 * (incluido el tamano real de la flash), lee el ADC1 y escanea redes WiFi.
 *
 * No necesita nada conectado.
 */

#include <WiFi.h>

#define LED_PIN 2        // LED de la placa en la mayoria de estas placas
#define PIN_ADC 34       // GPIO34 = ADC1_CH6. ADC1 anda con WiFi encendido

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(LED_PIN, OUTPUT);

  Serial.println();
  Serial.println("=== ESP32 USB-C CH340C ===");
  Serial.print("Chip:      ");
  Serial.println(ESP.getChipModel());
  Serial.print("Revision:  ");
  Serial.println(ESP.getChipRevision());
  Serial.print("Nucleos:   ");
  Serial.println(ESP.getChipCores());
  Serial.print("CPU:       ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");

  // Sirve para saber si tu lote trae 4, 8 o 16 MB
  Serial.print("Flash:     ");
  Serial.print(ESP.getFlashChipSize() / (1024 * 1024));
  Serial.println(" MB");

  Serial.print("RAM libre: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  Serial.print("MAC:       ");
  Serial.println(WiFi.macAddress());

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
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);
  delay(800);

  // GPIO34 es solo-entrada y no tiene pull interno: sin nada conectado
  // la lectura flota, es normal que salte
  int lectura = analogRead(PIN_ADC);
  Serial.print("ADC GPIO34: ");
  Serial.print(lectura);
  Serial.print("  (");
  Serial.print(lectura * 3.3 / 4095.0, 2);
  Serial.println(" V aprox)");
}
