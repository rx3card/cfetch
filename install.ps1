# Instalador de CFetch para Windows
# Requiere permisos de administrador

# Verificar si se está ejecutando como administrador
$isAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

if (-not $isAdmin) {
    Write-Host "Este script requiere permisos de administrador. Por favor, ejecuta PowerShell como administrador." -ForegroundColor Red
    exit 1
}

Write-Host "=== Instalador de CFetch ===" -ForegroundColor Cyan

# Directorios de instalación
$installDir = "$env:ProgramFiles\CFetch"
$binPath = "$installDir\cfetch.exe"
$configDir = "$env:APPDATA\cfetch"

# Crear directorios
Write-Host "Creando directorios..."
New-Item -ItemType Directory -Force -Path $installDir | Out-Null
New-Item -ItemType Directory -Force -Path $configDir | Out-Null

# Descargar la última versión (reemplaza con tu URL real)
$downloadUrl = "https://github.com/tuusuario/cfetch/releases/latest/download/cfetch-windows-x64.exe"
$downloadedFile = "$env:TEMP\cfetch-latest.exe"

Write-Host "Descargando la última versión de CFetch..."
try {
    Invoke-WebRequest -Uri $downloadUrl -OutFile $downloadedFile -UseBasicParsing
    
    # Mover el ejecutable al directorio de instalación
    Move-Item -Path $downloadedFile -Destination $binPath -Force
    
    # Agregar al PATH
    $currentPath = [Environment]::GetEnvironmentVariable('Path', 'Machine')
    if ($currentPath -notlike "*$installDir*") {
        [Environment]::SetEnvironmentVariable('Path', "$currentPath;$installDir", 'Machine')
    }
    
    # Crear archivo de configuración por defecto
    $configContent = @"
# Configuración de CFetch
# Colores disponibles: red, green, yellow, blue, magenta, cyan, white

color = "blue"
show_logo = true
show_title = true
show_dashes = true
"@
    
    $configContent | Out-File -FilePath "$configDir\config" -Encoding utf8
    
    Write-Host "`n¡CFetch se ha instalado correctamente!" -ForegroundColor Green
    Write-Host "Puedes usarlo escribiendo 'cfetch' en cualquier terminal nueva."
    Write-Host "La configuración se encuentra en: $configDir\config"
    
    # Preguntar si abrir la documentación
    $openDocs = Read-Host "¿Deseas abrir la documentación en el navegador? (s/n)"
    if ($openDocs -eq 's' -or $openDocs -eq 'S') {
        Start-Process "https://github.com/tuusuario/cfetch#readme"
    }
    
} catch {
    Write-Host "Error al descargar o instalar CFetch: $_" -ForegroundColor Red
    exit 1
}
