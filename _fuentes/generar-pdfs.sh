#!/usr/bin/env bash
# Regenera los 8 PDF a partir de los HTML de esta carpeta y los copia
# a su lugar en Productos/. Necesita Google Chrome instalado.
#
# Uso (desde Git Bash, parado en la raiz del repo):
#   bash _fuentes/generar-pdfs.sh

set -e
cd "$(dirname "$0")"
CHROME="/c/Program Files/Google/Chrome/Application/chrome.exe"
[ -x "$CHROME" ] || CHROME="/c/Program Files (x86)/Microsoft/Edge/Application/msedge.exe"

S="$(pwd)"
P="$S/../Productos"
V="$P/Vl53l0x Modulo Sensor De Serie Pwm Salida Láser"

for f in ds18b20_ficha c3_ficha ch340c_ficha vl53l0x_ficha ds18b20_guia c3_guia ch340c_guia vl53l0x_guia; do
  echo "  $f"
  "$CHROME" --headless --disable-gpu --no-sandbox --print-to-pdf="$S/$f.pdf" --no-pdf-header-footer "file:///$S/$f.html" >/dev/null 2>&1
done

cp "$S/ds18b20_ficha.pdf"  "$P/Sensor Digital Temperatura Ds18b20/ficha tecnica/DS18B20 - Ficha Tecnica.pdf"
cp "$S/ds18b20_guia.pdf"   "$P/Sensor Digital Temperatura Ds18b20/software/DS18B20 - Guia de Software.pdf"
cp "$S/c3_ficha.pdf"       "$P/Placa Desarrollo Esp32 C3 Super Mini/ficha tecnica/ESP32-C3 SuperMini - Ficha Tecnica.pdf"
cp "$S/c3_guia.pdf"        "$P/Placa Desarrollo Esp32 C3 Super Mini/software/ESP32-C3 SuperMini - Guia de Software.pdf"
cp "$S/ch340c_ficha.pdf"   "$P/Placa ESP32 USB C CH340C/ficha tecnica/ESP32 USB-C CH340C - Ficha Tecnica.pdf"
cp "$S/ch340c_guia.pdf"    "$P/Placa ESP32 USB C CH340C/software/ESP32 USB-C CH340C - Guia de Software.pdf"
cp "$S/vl53l0x_ficha.pdf"  "$V/ficha tecnica/GY-53 VL53L0X - Ficha Tecnica.pdf"
cp "$S/vl53l0x_guia.pdf"   "$V/software/GY-53 VL53L0X - Guia de Software.pdf"
rm -f "$S"/*.pdf
echo "Listo: 8 PDF regenerados y copiados a Productos/."
