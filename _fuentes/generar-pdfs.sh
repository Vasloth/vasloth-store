#!/usr/bin/env bash
# Regenera los 8 PDF a partir de los HTML de esta carpeta y los copia
# a su lugar en Productos/. Necesita Google Chrome (o Edge) instalado.
#
# Uso (desde Git Bash, parado en cualquier lado):
#   bash _fuentes/generar-pdfs.sh
#
# Genera en una carpeta temporal con ruta ASCII porque Chrome no acepta
# caracteres como ™ o á en una URL file:// sin codificar.

set -e
cd "$(dirname "$0")"
SRC="$(pwd)"
P="$SRC/../Productos"
V="$P/Vl53l0x Modulo Sensor De Serie Pwm Salida Láser"

CHROME="/c/Program Files/Google/Chrome/Application/chrome.exe"
[ -x "$CHROME" ] || CHROME="/c/Program Files (x86)/Microsoft/Edge/Application/msedge.exe"

T="$(mktemp -d)"
cp "$SRC"/*.html "$SRC"/estilo.css "$T/"
TW="$(cygpath -m "$T" 2>/dev/null || echo "$T")"

for f in ds18b20_ficha c3_ficha ch340c_ficha vl53l0x_ficha ds18b20_guia c3_guia ch340c_guia vl53l0x_guia; do
  echo "  $f"
  "$CHROME" --headless --disable-gpu --no-sandbox --print-to-pdf="$TW/$f.pdf" --no-pdf-header-footer "file:///$TW/$f.html" >/dev/null 2>&1
  # control: si Chrome no encontro el HTML, el PDF es una pagina de error
  if pdftotext "$T/$f.pdf" - 2>/dev/null | grep -q "ERR_FILE_NOT_FOUND"; then
    echo "ERROR: $f.pdf salio como pagina de error de Chrome. No se copia nada." >&2
    rm -rf "$T"; exit 1
  fi
done

cp "$T/ds18b20_ficha.pdf"  "$P/Sensor Digital Temperatura Ds18b20/ficha tecnica/DS18B20 - Ficha Tecnica.pdf"
cp "$T/ds18b20_guia.pdf"   "$P/Sensor Digital Temperatura Ds18b20/software/DS18B20 - Guia de Software.pdf"
cp "$T/c3_ficha.pdf"       "$P/Placa Desarrollo Esp32 C3 Super Mini/ficha tecnica/ESP32-C3 SuperMini - Ficha Tecnica.pdf"
cp "$T/c3_guia.pdf"        "$P/Placa Desarrollo Esp32 C3 Super Mini/software/ESP32-C3 SuperMini - Guia de Software.pdf"
cp "$T/ch340c_ficha.pdf"   "$P/Placa ESP32 USB C CH340C/ficha tecnica/ESP32 USB-C CH340C - Ficha Tecnica.pdf"
cp "$T/ch340c_guia.pdf"    "$P/Placa ESP32 USB C CH340C/software/ESP32 USB-C CH340C - Guia de Software.pdf"
cp "$T/vl53l0x_ficha.pdf"  "$V/ficha tecnica/VL53L0X - Ficha Tecnica.pdf"
cp "$T/vl53l0x_guia.pdf"   "$V/software/VL53L0X - Guia de Software.pdf"
rm -rf "$T"
echo "Listo: 8 PDF regenerados y copiados a Productos/."
