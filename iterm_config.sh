#!/bin/bash

# Colores ANSI
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # Sin color

# Configuración del entorno para trabajar con LiteX y FPGA
WORKDIR=~/Documents/UN/digital/litex-src
PDF_FILE="$WORKDIR/digital-course.pdf"
SERIAL_DEVICES=$(ls /dev/tty.usbserial* 2>/dev/null)

echo -e "${BLUE}🔧 Iniciando workspace para LiteX y FPGA...${NC}"
cd "$WORKDIR/src" || { echo -e "${RED}❌ No se pudo acceder al directorio src.${NC}"; exit 1; }

# Limpiar compilaciones anteriores
make clean && echo -e "${GREEN}✅ Archivos de compilación limpiados.${NC}"

# Volver al directorio base
cd "$WORKDIR" || exit

# Mostrar puertos seriales disponibles
echo -e "\n${CYAN}🔌 Puertos serial detectados:${NC}"
if [ -n "$SERIAL_DEVICES" ]; then
    echo "$SERIAL_DEVICES"
else
    echo -e "${YELLOW}⚠️  No se detectaron dispositivos seriales.${NC}"
fi

# Instrucciones
echo -e "${BLUE}
📘 Instrucciones de uso:
  - python base         → Construye la arquitectura con LiteX (Softcore).
  - make configure      → Carga el Softcore en la FPGA.
  - make litex_term     → Carga las aplicaciones a la BIOS del RISC-V.
${NC}"

# Preguntar si quiere abrir el PDF
echo -e "${CYAN}📄 ¿Deseas abrir el PDF del curso? (y/n)${NC}"
read -r ctl
if [[ "$ctl" == "y" || "$ctl" == "Y" ]]; then
    if [ -f "$PDF_FILE" ]; then
        open "$PDF_FILE"
    else
        echo -e "${YELLOW}⚠️  No se encontró el archivo PDF en: $PDF_FILE${NC}"
    fi
fi

# Referencia del proyecto
echo -e "\n${BLUE}🔗 Repositorio del proyecto:${NC}"
echo -e "${CYAN}   https://github.com/enjoy-digital/litex/tree/master/litex/soc/cores/cpu/picorv32${NC}"

# Ejecutar make configure
make configure

echo -e "\n${GREEN}✅ Entorno listo. ¡Buena sesión de desarrollo!${NC}\n"
