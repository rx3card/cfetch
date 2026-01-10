# Script de instalación mejorado para CFetch
$ErrorActionPreference = "Stop"

# Directorio de instalación
$installDir = "C:\Program Files\CFetch"
$exePath = "$installDir\cfetch.exe"

# Crear directorio de instalación
if (-not (Test-Path $installDir)) {
    New-Item -ItemType Directory -Path $installDir -Force | Out-Null
    Write-Host "Directorio de instalación creado: $installDir"
}

# Copiar el ejecutable
Copy-Item "build\cfetch.exe" -Destination $exePath -Force
Write-Host "Archivo copiado a: $exePath"

# Obtener el PATH actual
$currentPath = [Environment]::GetEnvironmentVariable('Path', 'Machine')

# Verificar si ya está en el PATH
if ($currentPath -notlike "*$installDir*") {
    # Agregar al PATH del sistema
    [Environment]::SetEnvironmentVariable('Path', "$currentPath;$installDir", 'Machine')
    Write-Host "Se ha agregado $installDir al PATH del sistema"
    
    # Actualizar el PATH en la sesión actual
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
} else {
    Write-Host "El directorio ya está en el PATH del sistema"
}

# Crear un acceso directo en el escritorio
$desktopPath = [Environment]::GetFolderPath("Desktop")
$shortcutPath = "$desktopPath\CFetch.lnk"
$WshShell = New-Object -comObject WScript.Shell
$Shortcut = $WshShell.CreateShortcut($shortcutPath)
$Shortcut.TargetPath = $exePath
$Shortcut.WorkingDirectory = $installDir
$Shortcut.Save()
Write-Host "Acceso directo creado en el escritorio"

# Mensaje final
Write-Host "`n¡CFetch se ha instalado correctamente!" -ForegroundColor Green
Write-Host "Puedes ejecutarlo desde cualquier terminal escribiendo 'cfetch'"
Write-Host "O haciendo doble clic en el acceso directo del escritorio"

# Preguntar si abrir la documentación
$openDocs = Read-Host "`n¿Deseas abrir la documentación en el navegador? (s/n)"
if ($openDocs -eq 's' -or $openDocs -eq 'S') {
    Start-Process "https://github.com/tuusuario/cfetch#readme"
}