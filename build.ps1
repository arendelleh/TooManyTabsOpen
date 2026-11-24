$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot
$compiler = 'C:\msys64\mingw64\bin\g++.exe'
if (!(Test-Path $compiler)) { $compiler = (Get-Command g++ -ErrorAction Stop).Source }
& $compiler -std=c++17 -Wall -Wextra -pedantic main.cpp -o TooManyTabsOpen.exe -lfreeglut -lopengl32
if ($LASTEXITCODE -ne 0) { throw 'Compilation failed.' }
Write-Host 'Built successfully. Run .\run.ps1'
