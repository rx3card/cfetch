#!/bin/bash

# Colores para la salida
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Instalando CFetch...${NC}"

# Verificar si es Linux o macOS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "Sistema detectado: Linux"
    sudo apt-get update
    sudo apt-get install -y build-essential git
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Sistema detectado: macOS"
    # Verificar si Homebrew está instalado
    if ! command -v brew &> /dev/null; then
        echo "Instalando Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    brew install gcc make
else
    echo "Sistema operativo no soportado."
    exit 1
fi

# Clonar el repositorio
echo "Descargando CFetch..."
git clone https://github.com/rx3card/cfetch.git
cd cfetch

# Compilar
echo "Compilando..."
make

# Instalar
echo "Instalando en /usr/local/bin (se requiere contraseña de administrador)..."
sudo cp cfetch /usr/local/bin/cfetch
sudo chmod +x /usr/local/bin/cfetch

# Limpiar
cd ..
rm -rf cfetch

echo -e "${GREEN}¡CFetch instalado correctamente!${NC}"
echo "Ejecuta 'cfetch' para ver la información de tu sistema."