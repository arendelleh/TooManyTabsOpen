param([switch]$Test, [string]$Output = 'TooManyTabsOpen.exe')
$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot
$compiler = 'C:\msys64\ucrt64\bin\g++.exe'
if (!(Test-Path $compiler)) { $compiler = 'C:\msys64\mingw64\bin\g++.exe' }
if (!(Test-Path $compiler)) { $compiler = (Get-Command g++ -ErrorAction Stop).Source }
$env:PATH = (Split-Path $compiler) + ';' + $env:PATH
$python = Join-Path (Split-Path $compiler) 'python.exe'
if (!(Test-Path $python)) { $python = (Get-Command python -ErrorAction Stop).Source }
& $python tools/generate_tasks.py
if ($LASTEXITCODE -ne 0) { throw 'Task content validation failed.' }
if (!(Test-Path 'assets/audio/forest_ambience.wav')) { & $python tools/generate_ambience.py }
& $compiler -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o $Output -lfreeglut -lglu32 -lopengl32 -lwinmm
if ($LASTEXITCODE -ne 0) { throw 'Compilation failed.' }
Write-Host 'Built successfully. Run .\run.ps1'
if ($Test) {
    & $python tools/generate_ambience.py --check
    if ($LASTEXITCODE -ne 0) { throw 'Ambience asset check failed.' }
    & $python -m unittest discover -s tests -p 'test_*.py'
    if ($LASTEXITCODE -ne 0) { throw 'Python content checks failed.' }
    & (Join-Path $PSScriptRoot $Output) --self-test
    if ($LASTEXITCODE -ne 0) { throw 'Gameplay checks failed.' }
}
