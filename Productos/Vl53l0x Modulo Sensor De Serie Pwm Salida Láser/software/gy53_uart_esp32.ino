/*
 * GY-53 (VL53L0X + STM32) - Lectura por puerto serie
 *
 * ESTE ES EL PROGRAMA RECOMENDADO PARA EL GY-53.
 *
 * El modulo arranca de fabrica en modo serie: el pin PS va SIN CONECTAR
 * (queda en alto). No hace falta ninguna libreria, porque el STM32 del
 * modulo hace las cuentas y manda la distancia ya calculada en mm.
 *
 * Si en vez de esto queres hablarle al chip por I2C, hay que poner PS a
 * GND y usar vl53l0x_i2c_esp32.ino (ahi si necesitas libreria).
 *
 * Trama de 8 bytes (manual GY-53 V1.0):
 *   [0]=0x5A [1]=0x5A [2]=0x15 [3]=0x03 [4]=dist_alto [5]=dist_bajo
 *   [6]=modo [7]=checksum (suma de los 7 anteriores, 8 bits bajos)
 *
 *   Distancia (mm) = (byte4 << 8) | byte5
 */

#define PIN_RX 16          // ESP32 clasico: RX del ESP32 <- TX del GY-53
#define PIN_TX 17          // ESP32 clasico: TX del ESP32 -> RX del GY-53
#define BAUDIOS 9600       // valor de fabrica del GY-53

HardwareSerial gy53(1);    // UART1 del ESP32

uint8_t trama[8];
int indice = 0;

// Declarada aca arriba para que tambien compile en PlatformIO
void enviarComando(uint8_t cmd);

void setup() {
  Serial.begin(115200);
  delay(500);

  gy53.begin(BAUDIOS, SERIAL_8N1, PIN_RX, PIN_TX);

  Serial.println("GY-53 en modo serie. Esperando datos...");

  // Salida continua (por si quedo en modo consulta)
  enviarComando(0x45);
  delay(100);

  // Modo de medicion: descomenta el que quieras
  // enviarComando(0x50);   // larga distancia: 0-2 m, 35 ms, +/-4 cm
  // enviarComando(0x51);   // rapido:          0-1.2 m, 22 ms, +/-3 cm
  enviarComando(0x52);      // alta precision:  0-1.2 m, 200 ms, +/-1 cm (default)
  // enviarComando(0x53);   // general:         0-1.2 m, 35 ms, +/-2 cm

  // Para que la configuracion sobreviva al apagado hay que guardarla.
  // Descomentar UNA vez, subir, y volver a comentar.
  // enviarComando(0x25);
}

// Los comandos son 0xA5 + comando + checksum
void enviarComando(uint8_t cmd) {
  uint8_t sum = 0xA5 + cmd;
  gy53.write(0xA5);
  gy53.write(cmd);
  gy53.write(sum);
}

void loop() {
  while (gy53.available()) {
    uint8_t b = gy53.read();

    // Sincronizacion: la trama arranca con 0x5A 0x5A
    if (indice == 0 && b != 0x5A) continue;
    if (indice == 1 && b != 0x5A) { indice = 0; continue; }

    trama[indice++] = b;

    if (indice == 8) {
      indice = 0;

      // Checksum: suma de los 7 primeros bytes, nos quedamos con 8 bits
      uint8_t suma = 0;
      for (int i = 0; i < 7; i++) suma += trama[i];

      if (suma != trama[7]) {
        Serial.println("Checksum invalido - descartada");
        continue;
      }

      uint16_t mm = ((uint16_t)trama[4] << 8) | trama[5];

      Serial.print(mm);
      Serial.print(" mm  (");
      Serial.print(mm / 10.0, 1);
      Serial.print(" cm)  modo=");

      switch (trama[6]) {
        case 0x00: Serial.println("larga distancia"); break;
        case 0x01: Serial.println("rapido");          break;
        case 0x02: Serial.println("alta precision");  break;
        case 0x03: Serial.println("general");         break;
        default:   Serial.println(trama[6], HEX);     break;
      }
    }
  }
}
