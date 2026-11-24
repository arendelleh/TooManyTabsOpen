$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot
if (Test-Path 'C:\msys64\mingw64\bin') { $env:PATH = 'C:\msys64\mingw64\bin;' + $env:PATH }
& .\TooManyTabsOpen.exe
