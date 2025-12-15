# ImGuiNodesExample (CMake) — v2

This version fixes the error you hit:

> missing `imgui-src/examples/libs/gl3w/GL/gl3w.c`

The ImGui repo layout changes over time, so depending on `examples/libs/gl3w` is fragile.

Instead, this project uses **ImGui's bundled OpenGL loader header** inside the backend:
`backends/imgui_impl_opengl3_loader.h`

So we do **not** compile gl3w/glad/glew at all.

## Build (creates a Visual Studio solution)

Open **cmd.exe** (or the VS "x64 Native Tools Command Prompt") and run:

```bat
cd C:\Users\Cosmos\Downloads\ImGuiNodes_CMake_Starter_v2\ImGuiNodes_CMake_Starter_v2
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Run:

```bat
.\build\Release\ImGuiNodesExample.exe
```

If CMake complains about Git, install Git for Windows and ensure `git` is in PATH.


## Note
If you modify docking calls, the current ImGui docking API expects:
`ImGui::DockSpaceOverViewport(dockspace_id, ImGui::GetMainViewport())`.
