# CFetch

[![GitHub release](https://img.shields.io/github/v/release/rx3card/cfetch)](https://github.com/rx3card/cfetch/releases)

Neofetch escrito en C puro. Rápido, ligero y sin dependencias.

**NOTA:** CFetch no es tan completo como neofetch, pero cubre los sistemas operativos más usados:
- Windows
- Linux:
   - Archlinux
   - Ubuntu
   - Debian

En otras distribuciones, se mostrará el logo genérico de Linux (el pingüino). Si quieres añadir soporte para más distribuciones, ¡tus contribuciones son bienvenidas!

## Instalación

### Linux y macOS

```bash
curl -sSL https://raw.githubusercontent.com/rx3card/cfetch/main/install-cfetch.sh | bash
```

O si prefieres descargar primero el script:

```bash
curl -O https://raw.githubusercontent.com/rx3card/cfetch/main/install-cfetch.sh
chmod +x install-cfetch.sh
./install-cfetch.sh
```

### Windows

1. Descarga el instalador de [la última versión](https://github.com/rx3card/cfetch/releases)
2. Ejecuta el archivo descargado
3. Sigue las instrucciones en pantalla
4. Abre una terminal y escribe `cfetch`

## Uso

```
cfetch          # Muestra la información del sistema
cfetch --help   # Muestra la ayuda
```

## Capturas de pantalla

![CFetch en Windows](screenshots/windows.png)
*Windows 10/11*

![CFetch en Archcraft](screenshots/archcraft.png)
*Linux (Archcraft)*

## Contribuir

Si quieres mejorar CFetch, ¡eres bienvenido! Solo haz un fork y envía un pull request.
