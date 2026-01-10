# Crear directorio de salida
New-Item -ItemType Directory -Force -Path ".\dist" -ErrorAction SilentlyContinue | Out-Null
New-Item -ItemType Directory -Force -Path ".\build" -ErrorAction SilentlyContinue | Out-Null

# Compilar el ejecutable
Write-Host "Compilando CFetch..." -ForegroundColor Cyan
gcc -o "build\cfetch.exe" cfetch.c -static

# Crear el instalador
Write-Host "`nBuscando Inno Setup..." -ForegroundColor Cyan

# Lista de posibles ubicaciones de Inno Setup
$possiblePaths = @(
    # Ruta específica de instalación
    "C:\Program Files (x86)\Inno Setup 6\ISCC.exe",
    # Otras ubicaciones comunes
    "${env:ProgramFiles(x86)}\Inno Setup 6\ISCC.exe",
    "${env:ProgramFiles}\Inno Setup 6\ISCC.exe"
)

$innoPath = $null

# Buscar en las ubicaciones posibles
foreach ($path in $possiblePaths) {
    if (Test-Path $path) {
        $innoPath = $path
        break
    }
}

# Verificar si se encontró Inno Setup
if ($innoPath) {
    Write-Host "Inno Setup encontrado en: $innoPath" -ForegroundColor Green
    
    try {
        # Crear el instalador
        Write-Host "`nCreando instalador..." -ForegroundColor Cyan
        & "$innoPath" "cfetch.iss"
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "`n✅ ¡Instalador creado exitosamente en la carpeta 'dist'!" -ForegroundColor Green
            $installerPath = (Get-Item "dist\CFetch-Setup.exe").FullName
            Write-Host "Instalador: $installerPath" -ForegroundColor Cyan
            
            # Preguntar si desea abrir la carpeta
            $openFolder = Read-Host "`n¿Deseas abrir la carpeta del instalador? (s/n)"
            if ($openFolder -eq 's' -or $openFolder -eq 'S') {
                explorer "$PWD\dist"
            }
        } else {
            Write-Host "`n❌ Error al crear el instalador. Código de salida: $LASTEXITCODE" -ForegroundColor Red
        }
    } catch {
        Write-Host "`n❌ Error al ejecutar Inno Setup: $_" -ForegroundColor Red
    }
} else {
    Write-Host "`n❌ Error: No se pudo encontrar Inno Setup en las ubicaciones comunes." -ForegroundColor Red
    Write-Host "Por favor, asegúrate de que Inno Setup esté instalado correctamente." -ForegroundColor Yellow
    Write-Host "Puedes descargarlo de: https://jrsoftware.org/isdl.php" -ForegroundColor Cyan
    
    # Intentar abrir el navegador para descargar
    $download = Read-Host "¿Deseas abrir la página de descarga de Inno Setup? (s/n)"
    if ($download -eq 's' -or $download -eq 'S') {
        Start-Process "https://jrsoftware.org/isdl.php"
    }
}