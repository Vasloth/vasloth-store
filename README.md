# Vasloth Store — Documentación de productos

Fichas técnicas, guías de conexión y código de ejemplo de los componentes que vendemos. Todo verificado contra los datasheets oficiales de cada fabricante, con la fuente citada en cada dato.

Cada producto tiene:

- **Ficha técnica** (PDF): qué es, pinout dibujado, especificaciones con fuente, para qué sirve, contra qué compite, y qué lo hace fallar.
- **Guía de software** (PDF): conexión pin a pin a ESP32, librerías, pasos para correrlo y errores comunes con su solución.
- **Código de ejemplo** (`.ino`): programa mínimo y comentado para Arduino IDE, listo para subir.

---

## Productos

### 🔵 Placa ESP32 USB-C · CH340C (30 pines)
Placa de desarrollo con ESP32-WROOM-32: doble núcleo, WiFi + Bluetooth clásico y BLE, USB-C.

- [Ficha técnica](Productos/Placa%20ESP32%20USB%20C%20CH340C/ficha%20tecnica/ESP32%20USB-C%20CH340C%20-%20Ficha%20Tecnica.pdf)
- [Guía de software](Productos/Placa%20ESP32%20USB%20C%20CH340C/software/ESP32%20USB-C%20CH340C%20-%20Guia%20de%20Software.pdf)
- [Código de ejemplo](Productos/Placa%20ESP32%20USB%20C%20CH340C/software/esp32_ch340c_test.ino)

### 🟢 Placa ESP32-C3 SuperMini
Placa compacta de 22,5 × 18 mm con ESP32-C3: RISC-V, WiFi + Bluetooth 5 LE, USB-C nativo.

- [Ficha técnica](Productos/Placa%20Desarrollo%20Esp32%20C3%20Super%20Mini/ficha%20tecnica/ESP32-C3%20SuperMini%20-%20Ficha%20Tecnica.pdf)
- [Guía de software](Productos/Placa%20Desarrollo%20Esp32%20C3%20Super%20Mini/software/ESP32-C3%20SuperMini%20-%20Guia%20de%20Software.pdf)
- [Código de ejemplo](Productos/Placa%20Desarrollo%20Esp32%20C3%20Super%20Mini/software/esp32c3_supermini_test.ino)

> ⚠️ Antes de usarla: en el IDE de Arduino activá **USB CDC On Boot → Enabled**. Sin eso, el Monitor Serie no muestra nada. Está explicado en la guía.

### 🌡️ Sensor de temperatura DS18B20 — sonda sumergible
Termómetro digital 1-Wire en vaina de acero inoxidable. −55 a +125 °C, ±0,5 °C.

- [Ficha técnica](Productos/Sensor%20Digital%20Temperatura%20Ds18b20/ficha%20tecnica/DS18B20%20-%20Ficha%20Tecnica.pdf)
- [Guía de software](Productos/Sensor%20Digital%20Temperatura%20Ds18b20/software/DS18B20%20-%20Guia%20de%20Software.pdf)
- [Código de ejemplo](Productos/Sensor%20Digital%20Temperatura%20Ds18b20/software/ds18b20_esp32.ino)

> ⚠️ Necesita una **resistencia de 4,7 kΩ** entre el cable de datos y el de alimentación. No viene incluida y sin ella no funciona.

### 📏 Sensor láser de distancia GY-53 (VL53L0X)
Medidor de distancia por tiempo de vuelo, hasta 2 m, con microcontrolador propio. Salida serie, PWM e I²C.

- [Ficha técnica](Productos/Vl53l0x%20Modulo%20Sensor%20De%20Serie%20Pwm%20Salida%20L%C3%A1ser/ficha%20tecnica/GY-53%20VL53L0X%20-%20Ficha%20Tecnica.pdf)
- [Guía de software](Productos/Vl53l0x%20Modulo%20Sensor%20De%20Serie%20Pwm%20Salida%20L%C3%A1ser/software/GY-53%20VL53L0X%20-%20Guia%20de%20Software.pdf)
- [Código — modo serie (recomendado)](Productos/Vl53l0x%20Modulo%20Sensor%20De%20Serie%20Pwm%20Salida%20L%C3%A1ser/software/gy53_uart_esp32.ino)
- [Código — modo I²C](Productos/Vl53l0x%20Modulo%20Sensor%20De%20Serie%20Pwm%20Salida%20L%C3%A1ser/software/vl53l0x_i2c_esp32.ino)

> ⚠️ Viene con una **película protectora** sobre el sensor. Sacala antes de usarlo o no va a medir.

---

## Cómo usar el código de ejemplo

1. Instalá el [IDE de Arduino](https://www.arduino.cc/en/software).
2. Agregá el soporte de placas ESP32: `Archivo → Preferencias → URLs adicionales` → pegá `https://espressif.github.io/arduino-esp32/package_esp32_index.json`, después `Herramientas → Placa → Gestor de tarjetas` → buscá **esp32** → instalar.
3. Abrí el `.ino` del producto, seguí la guía de software para el cableado y las librerías, y subilo.

Todos los ejemplos imprimen el resultado por el Monitor Serie a **115200 baudios**.

## Fuentes

Los datos técnicos salen de los documentos oficiales de cada fabricante:

| Componente | Documento |
|---|---|
| ESP32-WROOM-32 | Espressif, *ESP32-WROOM-32 Datasheet* v3.7 |
| ESP32-C3 | Espressif, *ESP32-C3 Family Datasheet* V1.0 |
| CH340C | WCH, *CH340 Datasheet* v3B |
| DS18B20 | Dallas / Maxim / Analog Devices, *DS18B20 Datasheet* |
| VL53L0X | STMicroelectronics, *VL53L0X Datasheet* DocID029104 Rev 2 |
| GY-53 | *GY-53 Infrared ranging module manual* V1.0 |
| Pines por defecto en Arduino | `arduino-esp32`, archivos `pins_arduino.h` y `boards.txt` |

Cada tabla de la ficha indica la tabla o sección exacta de donde sale cada valor.

---

<sub>Los archivos en `_fuentes/` son el HTML y CSS con los que se generan los PDF. No hace falta tocarlos para usar la documentación.</sub>
