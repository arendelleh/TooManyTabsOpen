# Source guide for the walkthrough

All listed files implement real project features. This guide covers the current 3D game source.

| File | Responsibility | What to show |
| --- | --- | --- |
| main.cpp | Original task logic, journal, GLUT callbacks, input dispatch, resize handling, and gameplay self-tests | reset, completeStep, keyboard, mouse, timer, main |
| lanternwood.hpp | 3D camera, fairy model, task scrolls, sanctuary, clickable UI, ray projection, and movement | scene3D, sceneClick, handleWorldClick, worldTick |
| activities.hpp | Contextual E actions, pickup/delivery targets, and in-world rest | activityTarget, performActivity, nearbyPrompt |
| activity_models.hpp | Collectible pages, planks, seeds, growing flowers, and visible task progress | activityModels and carried objects |
| storybook_ui.hpp | Mutually exclusive drawers, open-tab counts, and a single contextual action card | tabsDrawer, guideDrawer, actionReady, handleWorldClick |
| interface_font.hpp | Windows runtime Segoe UI texture atlas and portable GLUT fallback | prepare and draw |
| characters.hpp | Three keeper models and the clickable character/season panel | fairy, wardrobePanel, wardrobeClick |
| forest_detail.hpp | Seasonal broadleaf trees, branching trunks, flowers, pond, bench, deer, and butterflies | broadleaf, flowerPatch, forestDetails |
| fairy_lights.hpp | Hanging lantern models, sagging light strings, generated glow texture, additive halos, and animated glints | lantern, fairyLightGeometry, fairyGlowPass |
| forest_shader.hpp | GLSL compilation, linking, uniforms, and a fixed-pipeline fallback | init, stage, begin |
| assets/shaders/forest.vert | Transforms vertices and passes surface attributes to the fragment shader | main |
| assets/shaders/forest.frag | Surface lighting, rim light, and theme-dependent atmospheric fog | daylight uniform and main |
| ambience.hpp | Optional Windows background sound, on/off state, and clean stop | toggle and stop |
| tools/generate_tasks.py | Validates editable task data and generates task_data.hpp | validate and generate |
| assets/tasks.json | Four English task tabs and their example steps | titles and steps |
| tools/generate_ambience.py | Synthesizes the original 24-second major-key harp loop and verifies its WAV format | generate and verify |
| tests/test_content.py | Checks valid data, missing steps, duplicate IDs, unsupported text, and quote escaping | ContentTests |
| build.ps1 / run.ps1 | Windows compilation, tests, runtime DLL path, and launching | compiler arguments and commands |
| .github/workflows/build.yml | GitHub Actions Linux build and automated checks | job steps |

## Explain the actual graphics

- Geometry uses OpenGL/freeglut compatibility rendering, depth testing, perspective projection, lighting, and isolated matrix stacks. The original diamond demonstrates indexed vertex arrays.
- GLSL supplies custom surface lighting and fog. It does not add geometric detail by itself.
- The glow texture is generated in memory. Additive, camera-facing quads create soft fairy-light halos; line glints create the crisp sparkles.
- Task clicks project world positions into screen space. Ground clicks unproject a ray and intersect it with the ground plane. The fairy walks toward that point.
- The story lantern count represents game progression, not a measurement of mental health. Rest, quiet, boundaries, and task steps are valid choices.

## Verification

Run `./build.ps1 -Test` on the configured Windows PC. It checks task data, the audio file, five Python content cases, and C++ gameplay assertions. These checks do not prove how the game looks or sounds on every device. Record actual visual and audio checks separately.

