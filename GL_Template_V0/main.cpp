#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <cstdlib> // std::atoi

// -----------------------------
// OpenGL / GLFW / GLEW
// -----------------------------
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// -----------------------------
// stb_image & stb_image_write (optional here)
// -----------------------------
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../External_libs/stb/image/stb_image.h"
#include "../External_libs/stb/image/stb_image_write.h"

// -----------------------------
// glm (optional)
// -----------------------------
#include "../External_libs/glm_0_9_9_7/glm/glm/glm.hpp"

// -----------------------------
// Dear ImGui (docking branch, unity-build style)
// -----------------------------
//
// IMPORTANT:
//  - IMGUI_DEFINE_MATH_OPERATORS must be defined BEFORE imgui.h
//  - IMGUI_IMPL_OPENGL_LOADER_CUSTOM tells imgui_impl_opengl3.cpp
//    *not* to use its internal loader (imgui_impl_opengl3_loader.h),
//    and instead rely on whatever GL loader you already use (GLEW here).
//
// This avoids the conflict that causes PFNGLBINDTEXTUREPROC to be undefined
// when mixing GLEW + ImGui's embedded loader in one translation unit.
//
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

// Headers
#include "../External_libs/imgui-docking/imgui-docking/imgui.h"
#include "../External_libs/imgui-docking/imgui-docking/backends/imgui_impl_glfw.h"
#include "../External_libs/imgui-docking/imgui-docking/backends/imgui_impl_opengl3.h"

// Implementation (all compiled into this TU)
#include "../External_libs/imgui-docking/imgui-docking/imgui.cpp"
#include "../External_libs/imgui-docking/imgui-docking/imgui_draw.cpp"
#include "../External_libs/imgui-docking/imgui-docking/imgui_widgets.cpp"
#include "../External_libs/imgui-docking/imgui-docking/imgui_tables.cpp"
#include "../External_libs/imgui-docking/imgui-docking/imgui_demo.cpp"
#include "../External_libs/imgui-docking/imgui-docking/backends/imgui_impl_glfw.cpp"
#include "../External_libs/imgui-docking/imgui-docking/backends/imgui_impl_opengl3.cpp"


// Helper: GLFW error callback
static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main()
{
    std::cout << "ImGui + OpenGL Template\n";

    // ---------------------------------------------------------
    // 1. Initialize GLFW and create an OpenGL context
    // ---------------------------------------------------------
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Setup OpenGL version (3.3 core profile for ImGui OpenGL3 backend)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for macOS

    GLFWwindow* window =
        glfwCreateWindow(1280, 720, "ImGui + OpenGL Example", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // ---------------------------------------------------------
    // 2. Initialize GLEW (must be after context creation)
    // ---------------------------------------------------------
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: "
            << reinterpret_cast<const char*>(glewGetErrorString(err))
            << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    // ---------------------------------------------------------
    // 3. Setup Dear ImGui context
    // ---------------------------------------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Enable some ImGui features
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Multi-viewport (optional)

    // ImGui style
    ImGui::StyleColorsDark();

    // Adjust style if multi-viewports are enabled (optional)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // ---------------------------------------------------------
    // 4. Setup platform/renderer bindings (GLFW + OpenGL3)
    // ---------------------------------------------------------
    const char* glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // ---------------------------------------------------------
    // 5. Demo variables (this is your "UI state")
    // ---------------------------------------------------------
    bool show_demo_window = true;
    bool show_control_window = true;

    ImVec4 clear_color = ImVec4(0.10f, 0.15f, 0.20f, 1.00f);
    float  background_brightness = 1.0f;
    float  some_param = 0.5f;

    // ---------------------------------------------------------
    // 6. Main loop
    // ---------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // Poll OS events (input, window resize, etc.)
        glfwPollEvents();

        // 6.1 Start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // -------------------------------------------------
        // 6.2 (Optional) Main menu bar
        // -------------------------------------------------
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Quit", "Alt+F4"))
                {
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Windows"))
            {
                ImGui::MenuItem("Control Panel", nullptr, &show_control_window);
                ImGui::MenuItem("ImGui Demo", nullptr, &show_demo_window);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // -------------------------------------------------
        // 6.3 Your own main control window
        // -------------------------------------------------
        if (show_control_window)
        {
            ImGui::Begin("Control Panel", &show_control_window);

            ImGui::Text("Hello from ImGui in your GL template!");
            ImGui::Separator();

            ImGui::Text("Background");
            ImGui::ColorEdit3("Clear color", (float*)&clear_color);
            ImGui::SliderFloat("Brightness", &background_brightness, 0.0f, 2.0f);

            ImGui::Separator();
            ImGui::Text("Scene parameters");
            ImGui::SliderFloat("Some param", &some_param, 0.0f, 1.0f);
            ImGui::Text("Use this to drive your OpenGL uniforms,\n"
                "camera params, etc.");

            ImGui::Separator();
            ImGui::Text("IO");
            ImGui::Text("Mouse: (%.1f, %.1f)", io.MousePos.x, io.MousePos.y);
            ImGui::Text("FPS: %.1f", io.Framerate);

            ImGui::End();
        }

        // -------------------------------------------------
        // 6.4 Show Dear ImGui demo window (optional)
        // -------------------------------------------------
        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        // -------------------------------------------------
        // 6.5 Render your OpenGL scene *under* the UI
        // -------------------------------------------------
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        // Apply brightness scale
        float r = clear_color.x * background_brightness;
        float g = clear_color.y * background_brightness;
        float b = clear_color.z * background_brightness;

        glClearColor(r, g, b, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Here you would:
        //   - Bind VAOs/VBOs
        //   - Use your shader program
        //   - Pass 'some_param' as a uniform, etc.
        //
        // For now, we only clear the screen.

        // -------------------------------------------------
        // 6.6 Render ImGui on top of the scene
        // -------------------------------------------------
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // If using viewports (optional)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // ---------------------------------------------------------
    // 7. Cleanup
    // ---------------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
