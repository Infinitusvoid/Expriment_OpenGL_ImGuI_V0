// main.cpp
// Minimal "it just works" starter for:
//   - GLFW + OpenGL3
//   - Dear ImGui (docking + viewports) via CMake FetchContent
//   - ImNodes (node editor widgets)
//
// IMPORTANT:
// We do NOT use gl3w/glad/glew in this starter.
// ImGui's OpenGL backend uses its bundled loader header internally:
//   backends/imgui_impl_opengl3_loader.h
// That keeps this project stable even when ImGui repo paths change.

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <algorithm>
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imnodes.h"

// GLFW error callback
static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << "\n";
}

struct Link
{
    int id = 0;
    int start_attr = 0;
    int end_attr = 0;
};

static int NextId()
{
    static int s = 1;
    return s++;
}

int main()
{
    std::cout << "ImGui (docking+viewports) + ImNodes (CMake FetchContent)\n";

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return -1;

    // Request OpenGL 3.3 core (works with ImGui OpenGL3 backend)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui + ImNodes (CMake)", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";

    // -----------------------------
    // ImGui setup
    // -----------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // -----------------------------
    // ImNodes setup
    // -----------------------------
    ImNodes::CreateContext();
    ImNodes::StyleColorsDark();

    // Demo graph state
    const int nodeA = NextId();
    const int nodeB = NextId();

    const int nodeA_out = NextId();
    const int nodeB_in  = NextId();

    std::vector<Link> links;
    int next_link_id = 1;

    bool show_demo = false;
    ImVec4 clear_color(0.10f, 0.15f, 0.20f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui docking API changed across versions; use the ID+viewport signature.
        ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpaceOverViewport(dockspace_id, main_viewport);

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Quit", "Alt+F4"))
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Windows"))
            {
                ImGui::MenuItem("ImGui Demo", nullptr, &show_demo);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::Begin("Control Panel");
        ImGui::ColorEdit3("Clear", (float*)&clear_color);
        ImGui::Text("Tips:");
        ImGui::BulletText("MMB drag to pan");
        ImGui::BulletText("Mouse wheel to zoom");
        ImGui::BulletText("Drag from pin to pin to create a link");
        ImGui::End();

        ImGui::Begin("Node Editor");

        ImNodes::BeginNodeEditor();

        ImNodes::BeginNode(nodeA);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Input");
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginOutputAttribute(nodeA_out);
        ImGui::Indent(40.0f);
        ImGui::TextUnformatted("out");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        ImNodes::BeginNode(nodeB);
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Output");
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(nodeB_in);
        ImGui::TextUnformatted("in");
        ImNodes::EndInputAttribute();

        ImNodes::EndNode();

        for (const Link& l : links)
            ImNodes::Link(l.id, l.start_attr, l.end_attr);

        ImNodes::MiniMap(0.20f, ImNodesMiniMapLocation_BottomRight);

        ImNodes::EndNodeEditor();

        // New links
        int start_attr = 0, end_attr = 0;
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
        {
            const bool exists = std::any_of(links.begin(), links.end(), [&](const Link& l)
            {
                return (l.start_attr == start_attr && l.end_attr == end_attr) ||
                       (l.start_attr == end_attr   && l.end_attr == start_attr);
            });

            if (!exists)
                links.push_back({ next_link_id++, start_attr, end_attr });
        }

        // Deleted links
        int destroyed_link_id = 0;
        if (ImNodes::IsLinkDestroyed(&destroyed_link_id))
        {
            links.erase(std::remove_if(links.begin(), links.end(), [&](const Link& l)
            {
                return l.id == destroyed_link_id;
            }), links.end());
        }

        ImGui::End(); // Node Editor

        if (show_demo)
            ImGui::ShowDemoWindow(&show_demo);

        // Render
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup);
        }

        glfwSwapBuffers(window);
    }

    ImNodes::DestroyContext();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
