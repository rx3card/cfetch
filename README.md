# CFetch - Herramienta de Información del Sistema

![CFetch en acción](https://i.imgur.com/ejemplo.png)

CFetch es una herramienta ligera de línea de comandos que muestra información detallada del sistema con un estilo visual atractivo, similar a neofetch pero más simple y rápida.

## 🚀 Características

- Muestra información esencial del sistema operativo
- Interfaz colorida y minimalista
- Rápida y ligera
- Sin dependencias externas en Windows
- Fácil de instalar y usar
- Instalador automático para Windows
- Compatible con Windows, Linux y macOS

## 💻 Requisitos

### Windows
- Windows 7 o superior
- Opcional: MinGW o Visual Studio (para compilación manual)

### Linux/macOS
- gcc o clang instalado
- make (para instalación fácil)

### Espacio en disco: Menos de 1MB

## 🛠️ Instalación en Windows

### Método 1: Instalador Automático (Recomendado)
1. Descarga el archivo `CFetch_Setup.exe` más reciente de la sección [Releases](https://github.com/tuusuario/cfetch/releases)
2. Ejecuta el instalador y sigue las instrucciones en pantalla
3. Abre una nueva terminal y escribe `cfetch`

### Método 2: Instalación Manual

#### Requisitos previos:
- PowerShell 5.1 o superior
- Acceso de administrador (recomendado para instalación en todo el sistema)

#### Pasos:
1. **Clona el repositorio** o descarga el código fuente
   ```powershell
   git clone https://github.com/tuusuario/cfetch.git
   cd cfetch
   ```

2. **Ejecuta el script de compilación**:
   ```powershell
   .\build_simple.ps1
   ```
   Esto creará un archivo `cfetch.exe` en la carpeta `build/`

3. **Instala CFetch**:
   ```powershell
   .\install.ps1
   ```

4. **Verifica la instalación**:
   ```powershell
   cfetch
   ```

### Desinstalación
Para desinstalar CFetch, usa cualquiera de estos métodos:

#### Desde el Menú de Inicio:
1. Ve a Configuración > Aplicaciones
2. Busca "CFetch"
3. Haz clic en "Desinstalar"

#### Manualmente:
```powershell
# Elimina el ejecutable y accesos directos
Remove-Item -Path "$env:ProgramFiles\CFetch" -Recurse -Force -ErrorAction SilentlyContinue

# Elimina la entrada del PATH
$currentPath = [Environment]::GetEnvironmentVariable('Path', 'Machine')
$newPath = ($currentPath -split ';' | Where-Object { $_ -ne "$env:ProgramFiles\CFetch" }) -join ';'
[Environment]::SetEnvironmentVariable('Path', $newPath, 'Machine')
```

## 🐧 Linux/macOS

### Requisitos:
```bash
# En Ubuntu/Debian
sudo apt update && sudo apt install -y build-essential git

# En macOS (con Homebrew)
brew install gcc make
```

### Instalación:
```bash
# Clona el repositorio
git clone https://github.com/tuusuario/cfetch.git
cd cfetch

# Compila
make

# Instala (requiere sudo)
sudo make install

# Verifica la instalación
cfetch
```

### Desinstalación:
```bash
cd cfetch
sudo make uninstall
```

## 🚀 Uso

### Comando Básico
```bash
cfetch
```

### Opciones
```
cfetch                # Muestra la información del sistema
cfetch --help         # Muestra la ayuda
cfetch --version      # Muestra la versión
cfetch --no-color     # Desactiva los colores
cfetch --simple       # Modo simple (menos información)
```

## 🛠 Compilación Avanzada

### Windows (Compilación Manual)
1. Instala MinGW o Visual Studio con soporte para C++
2. Usa el script `build_simple.ps1` o compila manualmente:
   ```powershell
   gcc -o cfetch.exe cfetch.c -static
   ```

### Crear Instalador (Desarrolladores)
1. Instala [Inno Setup](https://jrsoftware.org/isdl.php)
2. Ejecuta:
   ```powershell
   .\build_installer.ps1
   ```
   Esto generará un instalador en la carpeta `dist/`

## 📦 Crear una Nueva Versión (Release)

1. Actualiza el número de versión en el código fuente
2. Crea un tag de Git:
   ```bash
   git tag -a v1.0.0 -m "Versión 1.0.0"
   git push origin v1.0.0
   ```
3. Crea un nuevo release en GitHub y adjunta el instalador generado

## 🤝 Contribuir

1. Haz un fork del proyecto
2. Crea una rama para tu característica (`git checkout -b feature/nueva-funcion`)
3. Haz commit de tus cambios (`git commit -am 'Añade nueva función'`)
4. Haz push a la rama (`git push origin feature/nueva-funcion`)
5. Abre un Pull Request

## 📂 Estructura del Proyecto

- `cfetch.c` - Código fuente principal
- `build_simple.ps1` - Script de compilación para Windows
- `install.ps1` - Instalador para Windows
- `Makefile` - Para compilación en Linux/macOS

## 🛠️ Desarrollo

### Compilación

**Windows (PowerShell):**
```powershell
.\build_simple.ps1
```

**Linux/macOS:**
```bash
make
```

## 📝 Licencia

Este proyecto está bajo la Licencia MIT. Ver el archivo [LICENSE](LICENSE) para más detalles.

## 🤝 Contribuir

Las contribuciones son bienvenidas. Por favor lee [CONTRIBUTING.md](CONTRIBUTING.md) para más detalles.

## 📧 Contacto

¿Preguntas o sugerencias? ¡Abre un issue o envíame un mensaje!

### Opciones
- `-h, --help`     Muestra la ayuda
- `-v, --version`  Muestra la versión
- `-c, --config`   Especifica un archivo de configuración

## Personalización

Puedes personalizar la salida creando un archivo de configuración en:
- Linux/macOS: `~/.config/cfetch/config`
- Windows: `%APPDATA%\cfetch\config`

## Compilar desde el código fuente

### Requisitos
- Compilador C (gcc, clang, o MSVC)
- Git
- CMake (opcional, recomendado)

```bash
git clone https://github.com/tuusuario/cfetch.git
cd cfetch
# Para Linux/macOS
./build.sh
# Para Windows
.\build.ps1
```

## Contribuir

¡Las contribuciones son bienvenidas! Por favor lee [CONTRIBUTING.md](CONTRIBUTING.md) para más detalles.

## Licencia

Este proyecto está licenciado bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para más detalles.
