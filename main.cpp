#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imnodes.h"
#include "editor.h"

int main(int argc, char* argv[]) {
    // Initialize SDL2
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

    // Create a window
    SDL_Window* window = SDL_CreateWindow("lambda", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    // Setup ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImNodes::CreateContext();
    ImGuiStyle& style = ImGui::GetStyle();
    ImNodesStyle& editor_style = ImNodes::GetStyle();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

    editor_style.Colors[ImNodesCol_GridBackground] = IM_COL32(50, 50, 50, 255);
    editor_style.Colors[ImNodesCol_GridLine] = IM_COL32(75, 75, 75, 255);
    editor_style.Colors[ImNodesCol_GridLinePrimary] = IM_COL32(100, 100, 100, 255);
    editor_style.Colors[ImNodesCol_MiniMapBackground] = IM_COL32(75, 75, 75, 200);
    editor_style.Colors[ImNodesCol_MiniMapBackgroundHovered] = IM_COL32(100, 100, 100, 200);
    editor_style.Colors[ImNodesCol_Link] = IM_COL32(100, 100, 100, 255);
    editor_style.Colors[ImNodesCol_LinkSelected] = IM_COL32(150, 150, 150, 255);
    editor_style.Colors[ImNodesCol_LinkHovered] = IM_COL32(150, 150, 150, 255);


    // Initialize ImGui for SDL2
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Setup style
    ImGui::StyleColorsDark();

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {quit = true;}     
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE or event.key.keysym.sym == SDLK_DELETE) {
                    delete_selected();
                }
            }
        }

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // Create ImGui content

        ImVec2 fullscreen_size = ImVec2((int)io.DisplaySize.x, (int)io.DisplaySize.y);
        ImGui::SetNextWindowSize(fullscreen_size);
        ImGui::SetNextWindowPos(ImVec2(0,0));

        ImGui::Begin("main", nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
        ImGui::DockSpace(ImGui::GetID("main"));

        ImGui::Begin("editor");

        run_editor();

        ImGui::End();
        ImGui::End();

        // Rendering
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.20f, 0.20f, 0.20f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImNodes::DestroyContext();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}