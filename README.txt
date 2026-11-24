TOO MANY TABS OPEN
C++17 | OpenGL/freeglut

ABOUT
A 2D fictional desktop for college students to reflect on academic overwhelm
and task organization. Choose a small task step, set work aside, mute a
simulated message, or take a breathing break.
It does not diagnose mental-health conditions or measure well-being.
Progress lasts only during the session. Reset or exit clears it.

HOW TO RUN
Requires a C++17 compiler and freeglut. The scripts use C:\msys64\mingw64.
Open the TooManyTabsOpen folder in VS Code, then choose:
Terminal > Run Task > Run Too Many Tabs Open
Press Ctrl+Shift+B to build only. Close the app before rebuilding.

Or run these commands in that folder's PowerShell terminal:
  powershell -ExecutionPolicy Bypass -File .\build.ps1
  powershell -ExecutionPolicy Bypass -File .\run.ps1
Use run.ps1 so Windows can find the required DLLs.

CONTROLS
Enter   Open the desktop from Welcome
1-4     Select a task, including in Focus mode
C       Acknowledge the next example step (up to three per task)
L       Set the task aside or bring it back
F       Show or hide other tabs
M       Mute or show the simulated message
B       Switch between Desktop and Break
Space   Pause or resume the breathing animation on Break
R       Reset and return to Welcome
Esc     Exit
You can also left-click tabs and buttons.

GRAPHICS AND CODE
Three screens: Welcome, Desktop, and Break.
Objects include task tabs, a checklist, a folded note, buttons, and a
breathing circle. Focus mode and breathing animation are creative features.
Functions organize drawing and actions; conditions control states and
prevent completing tasks set aside. Arrays and loops handle tasks and steps.
GLUT callbacks handle drawing, keyboard input, mouse clicks, resizing,
and animation. Bitmap text shows instructions and feedback.

Primitives: GL_QUADS, GL_LINES, GL_TRIANGLE_FAN, and GL_TRIANGLES.
The diamond stores four 2D corner vertices and six indices. glVertexPointer
and glDrawElements draw two triangles, reusing corners instead of repeating
vertex data. This needs a compatibility OpenGL context.

TESTING
With C:\msys64\mingw64\bin on PATH, run:
  .\TooManyTabsOpen.exe --self-test
This checks step limits, Later behavior, reset, and click boundaries.
Record actual manual results in TESTING.csv. The self-test does not check
visual layout or every keyboard and mouse interaction.
