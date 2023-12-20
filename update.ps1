# Obtener la ruta del script
$rutaDelScript = Split-Path -Parent $MyInvocation.MyCommand.Definition
Write-Host "Esperando cierre..."
Start-Sleep -Seconds 5

Write-Host "Compilando..."
# Agrega los archivos fuente según sea necesario
g++ window-handler\main.cpp window-handler\headers\rubLogger\rubLogger.cpp window-handler\headers\windowHandler\windowHandler.cpp window-handler\headers\updater\update.cpp -o RubBs.exe -mwindows

# Pregunta al usuario si desea agregar el programa al inicio de Windows
$respuesta = Read-Host "Quieres añadir el programa al startup de Windows? (Y/n)"

if ($respuesta -eq 'Y' -or $respuesta -eq 'y') {
    # Crea el objeto de acceso directo
    $shell = New-Object -ComObject WScript.Shell
    $shortcut = $shell.CreateShortcut("$env:APPDATA\Microsoft\Windows\Start Menu\Programs\Startup\RubBs.lnk")
    $shortcut.TargetPath = "$rutaDelScript\RubBs.exe"
    $shortcut.WorkingDirectory = $rutaDelScript  # Establecer el directorio de inicio
    $shortcut.Save()
} elseif ($respuesta -eq 'N' -or $respuesta -eq 'n') {
    Write-Host ":("
} else {
    # Si la respuesta no es ni Y ni N, asume que sí y crea el acceso directo
    $shell = New-Object -ComObject WScript.Shell
    $shortcut = $shell.CreateShortcut("$env:APPDATA\Microsoft\Windows\Start Menu\Programs\Startup\RubBs.lnk")
    $shortcut.TargetPath = "$rutaDelScript\RubBs.exe"
    $shortcut.WorkingDirectory = $rutaDelScript  # Establecer el directorio de inicio
    $shortcut.Save()
}

# Cambia al directorio del script antes de iniciar el programa
Set-Location -Path $rutaDelScript

# Inicia el programa
Start-Process -FilePath ".\RubBs.exe"

# Salir del script
exit
