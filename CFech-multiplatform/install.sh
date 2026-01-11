#!/bin/bash

# Colores
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # Sin color

# Verificar si se está ejecutando como root
if [ "$EUID" -ne 0 ]; then
    echo -e "${YELLOW}Por favor, ejecuta este script como root o con sudo${NC}"
    exit 1
fi

echo -e "${YELLOW}Instalando CFetch...${NC}"

# Crear directorio de instalación
INSTALL_DIR="/usr/local/bin"
BIN_NAME="cfetch"

# Detectar sistema operativo
OS="$(uname -s)"
case "$OS" in
    Linux*)
        echo "Sistema detectado: Linux"
        COMPILER="gcc"
        FLAGS="-D_OS_LINUX"
        ;;
    Darwin*)
        echo "Sistema detectado: macOS"
        COMPILER="clang"
        FLAGS="-D_OS_MACOS"
        ;;
    *)
        echo -e "${RED}Error: Sistema operativo no soportado${NC}"
        exit 1
        ;;
esac

# Verificar si el compilador está instalado
if ! command -v $COMPILER &> /dev/null; then
    echo -e "${RED}Error: $COMPILER no está instalado${NC}"
    if [ "$OS" = "Linux" ]; then
        echo "Instalando dependencias..."
        apt-get update && apt-get install -y build-essential
    else
        echo "Por favor instala Xcode Command Line Tools ejecutando: xcode-select --install"
        exit 1
    fi
fi

# Compilar
echo "Compilando CFetch..."
$COMPILER cfetch.c -o $BIN_NAME $FLAGS

if [ $? -ne 0 ]; then
    echo -e "${RED}Error al compilar CFetch${NC}"
    exit 1
fi

# Instalar
echo "Instalando en $INSTALL_DIR..."
mv $BIN_NAME $INSTALL_DIR/
chmod +x $INSTALL_DIR/$BIN_NAME

echo -e "${GREEN}✅ CFetch instalado correctamente!${NC}"
echo -e "Ejecuta 'cfetch' en cualquier terminal para ver la información del sistema."