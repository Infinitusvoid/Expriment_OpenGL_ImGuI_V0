cmake_minimum_required(VERSION 3.20)

project(ImGuiNodesExample LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# -----------------------------
# GLFW
# -----------------------------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

# -----------------------------
# Dear ImGui (docking branch)
# NOTE:
#  - We intentionally DO NOT build gl3w/glad/glew here.
#  - ImGui's OpenGL backend can use its bundled loader header:
#      backends/imgui_impl_opengl3_loader.h
#    (this avoids fragile paths like examples/libs/gl3w/... which may change).
# -----------------------------
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        docking
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

set(IMGUI_DIR "${imgui_SOURCE_DIR}")

add_library(imgui STATIC
  "${IMGUI_DIR}/imgui.cpp"
  "${IMGUI_DIR}/imgui_draw.cpp"
  "${IMGUI_DIR}/imgui_tables.cpp"
  "${IMGUI_DIR}/imgui_widgets.cpp"
  "${IMGUI_DIR}/imgui_demo.cpp"

  # Backends
  "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
  "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(imgui PUBLIC
  "${IMGUI_DIR}"
  "${IMGUI_DIR}/backends"
)

target_link_libraries(imgui PUBLIC glfw)

# -----------------------------
# ImNodes (node editor widgets for ImGui)
# -----------------------------
FetchContent_Declare(
  imnodes
  GIT_REPOSITORY https://github.com/Nelarius/imnodes.git
  GIT_TAG        master
)
FetchContent_GetProperties(imnodes)
if(NOT imnodes_POPULATED)
  FetchContent_Populate(imnodes)
endif()

add_library(imnodes STATIC
  "${imnodes_SOURCE_DIR}/imnodes.cpp"
)

target_include_directories(imnodes PUBLIC
  "${imnodes_SOURCE_DIR}"
  "${IMGUI_DIR}"
)

target_link_libraries(imnodes PUBLIC imgui)

# -----------------------------
# OpenGL
# -----------------------------
find_package(OpenGL REQUIRED)

# -----------------------------
# App
# -----------------------------
add_executable(ImGuiNodesExample
  main.cpp
)

target_link_libraries(ImGuiNodesExample PRIVATE
  imnodes
  imgui
  OpenGL::GL
)

target_compile_definitions(ImGuiNodesExample PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)

if(MSVC)
  target_compile_options(ImGuiNodesExample PRIVATE /W3 /permissive-)
else()
  target_compile_options(ImGuiNodesExample PRIVATE -Wall -Wextra -Wpedantic)
endif()
