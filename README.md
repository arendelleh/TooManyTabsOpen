# Too Many Tabs Open

A C++17 and OpenGL/freeglut project about academic overwhelm and making space for one small step.

## Current version

The current baseline is a 2D fictional student desktop with four task tabs, example action steps, a Later option, Focus mode, simulated notification muting, and an optional breathing-break screen. Tasks and progress last for the session.

This is an educational reflection activity. It does not diagnose conditions or measure well-being.

## Run

Open this folder in VS Code. Use **Terminal → Run Task → Run Too Many Tabs Open**. The existing scripts expect a C++17 compiler and freeglut; their original compiler location is `C:\msys64\mingw64\bin`.

See [README.txt](README.txt) for the original build instructions and complete controls. The executable and library binaries are excluded from Git.

## Main controls

| Input | Action |
| --- | --- |
| Enter | Open the desktop |
| 1–4 | Select a task |
| C / L | Acknowledge the next example step / set a task aside or bring it back |
| F / M | Toggle Focus mode / simulated notification muting |
| B / Space | Open or leave the break screen / pause its animation |
| R / Esc | Reset / exit |

## Verification

The source includes a `--self-test` mode for task-step limits, Later behavior, reset, and click boundaries. Record actual manual results in [TESTING.csv](TESTING.csv); blank results are not passing results.

## Planned development

The next update is a full 3D fairytale game adaptation that preserves the original theme and meaningful choices. This baseline commit does not yet implement the 3D update.
