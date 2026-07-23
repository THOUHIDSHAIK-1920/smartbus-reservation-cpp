param()
$projectRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
$buildDir = Join-Path $projectRoot "build"
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host "Error: CMake was not found in PATH. Install CMake and add it to PATH before running this script."
    exit 1
}
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}
Set-Location $buildDir
Write-Host "Configuring project in $buildDir"
cmake .. -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
Write-Host "Building project"
cmake --build . --config Release
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$exe = Join-Path $buildDir "Release\SmartBusReservationSystem.exe"
if (-not (Test-Path $exe)) {
    $exe = Join-Path $buildDir "SmartBusReservationSystem.exe"
}
Write-Host "Running application: $exe"
& $exe
