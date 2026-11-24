# Too Many Tabs Open

![C++17](assets/badges/cpp.svg) ![GLSL](assets/badges/glsl.svg) ![Python](assets/badges/python.svg) ![PowerShell](assets/badges/powershell.svg)
[![Build and gameplay checks](https://github.com/arendelleh/TooManyTabsOpen/actions/workflows/build.yml/badge.svg)](https://github.com/arendelleh/TooManyTabsOpen/actions/workflows/build.yml)

A full 3D fairytale reflection game about academic overwhelm and making room for one small choice.

## Two moods, one story

- **The Daydream Grove:** pastel skies, blossom-colored trees, butterflies, drifting petals, an awake woodland companion, and decorative lanterns switched off.
- **The Lanternwood:** moonlight, illuminated lantern cages, fairy-light strings, fireflies, moonflowers, and a resting deer.

Choose Summer, Autumn, or Snowfall independently of day and night. Summer has blossoms and butterflies; Autumn changes the canopy and drifting leaves; Snowfall covers the ground and crowns with snow. Choose a Petal Fairy, Woodland Elf, or Moon Mage in the storybook side panel.

Four task scrolls preserve the original college-desktop concept. Restore lost pages to a study book, carry planks to repair a bridge, plant seeds that grow into flowers, or prepare tea beside the pond. Each activity changes an object in the world. You can also set a concern aside, quiet the message moth, or rest. Light three story lanterns and return to the sanctuary. There is no countdown or failure penalty. The original desktop remains available as a task journal.

The lantern count is fictional story progression, not a mental-health score. The game does not diagnose conditions or measure well-being. Progress lasts for the current session.

## Run on the configured Windows PC

Open PowerShell in this project folder:

    .\build.ps1 -Test
    .\run.ps1

The build script prefers **C:\msys64\ucrt64\bin**, then checks the original mingw64 location or a compiler on PATH. It needs C++17, freeglut, GLU, OpenGL, and Python 3. Python validates task data before compilation. Keep the shader and audio assets with the application; run.ps1 sets the working directory and runtime DLL path.

## Clickable controls

| Input | Action |
| --- | --- |
| Enter | Start or continue exploring |
| Click ground / WASD | Walk to a point / walk manually |
| Click scroll or left-hand task button | Select its task and walk toward the next spark |
| E or Interact button | Perform the displayed action near the marked object: collect, carry, place, plant, or prepare tea |
| L / F / M | Set aside or restore a tab / Focus / quiet the message moth |
| B or pond | Visit the quiet glade |
| T / J | Toggle compact task details / original desktop journal |
| C / K | Open the character and season panel / cycle seasons |
| N or Day/Night button | Switch themes without losing progress |
| V or Sound button | Toggle the original background ambience |
| Right-drag / wheel | Orbit / zoom the 3D camera |
| H / F11 / R / Esc | Help / full-screen / restart / exit |

All game text is English. The game starts full-screen and fills the display without fixed-aspect side bars; F11 switches to a resizable window. Sound starts off. The original 24-second soft major-key harp loop is synthesized by tools/generate_ambience.py; it uses no downloaded recordings. Sound playback is currently implemented for Windows. Other platforms retain the visuals and gameplay.

## What each language does

| Language | Actual role |
| --- | --- |
| C++ | Gameplay, 3D geometry, camera, picking, UI, input, and sound control |
| GLSL | Custom surface lighting, rim light, and day/night atmospheric fog |
| Python | Task-content validation, generated C++ data, original audio synthesis, and content tests |
| PowerShell | Windows build, test, and launch scripts |

GitHub shows the dominant language on the repository list. The repository Languages section can show the other detected source languages; badges above describe their actual roles. Shader files are explicitly classified as GLSL in .gitattributes, and the generated task header is marked as generated.

Editable task data is in assets/tasks.json. The generated task_data.hpp is checked in so the source stays understandable. Technology badges describe the implementation; they are not GitHub achievements.

## Verification and automation

GitHub Actions installs Linux graphics development libraries, validates content, checks the audio format, compiles the game, and runs the C++ gameplay checks. The self-test runs without opening a window.

Local checks cover pickup and delivery for all four activities, task limits, Later behavior, proximity, lantern limits, rest and position restoration, sanctuary completion, movement, reset, theme buttons, character and season selection, and task navigation. Five Python tests cover content validation. Device-specific visual and audio checks remain part of manual testing.

See [the source guide](docs/Code%20Guide.md) for a file-by-file explanation.

## Development history

The original 2D desktop is preserved in baseline commit **8022581**. The 3D adaptation is developed on **codex/lanternwood-3d**, with working updates committed separately. No license has been added; upstream library licenses still apply.
