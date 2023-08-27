#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imnodes.h"
#include "editor.h"

int main(int argc, char* argv[]) {

    nodes.push_back(new start_node(base_node_id++,base_pin_id++));
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
    io.Fonts->AddFontFromFileTTF("fonts/OpenSans_SemiCondensed-Medium.ttf", 18);
    io.IniFilename = "lambda_layout";

    style.FrameRounding = 5.0f;

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 0.9399999976158142f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.09441966563463211f, 0.09442012012004852f, 0.09442061185836792f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.09441966563463211f, 0.09442012012004852f, 0.09442061185836792f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2231759428977966f, 0.2231737077236176f, 0.2231737077236176f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.09441966563463211f, 0.09442012012004852f, 0.09442061185836792f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2231759428977966f, 0.2231737077236176f, 0.2231737077236176f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.2231759428977966f, 0.2231737077236176f, 0.2231737077236176f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.2231759428977966f, 0.2231737077236176f, 0.2231737077236176f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.2231759428977966f, 0.2231737077236176f, 0.2231737077236176f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);

    editor_style.Colors[ImNodesCol_GridBackground] = IM_COL32(50, 50, 50, 255);
    editor_style.Colors[ImNodesCol_GridLine] = IM_COL32(75, 75, 75, 255);
    editor_style.Colors[ImNodesCol_GridLinePrimary] = IM_COL32(100, 100, 100, 255);
    editor_style.Colors[ImNodesCol_MiniMapBackground] = IM_COL32(75, 75, 75, 200);
    editor_style.Colors[ImNodesCol_MiniMapBackgroundHovered] = IM_COL32(100, 100, 100, 200);
    editor_style.Colors[ImNodesCol_Link] = IM_COL32(120, 120, 120, 255);
    editor_style.Colors[ImNodesCol_LinkSelected] = IM_COL32(150, 150, 150, 255);
    editor_style.Colors[ImNodesCol_LinkHovered] = IM_COL32(150, 150, 150, 255);
    editor_style.Colors[ImNodesCol_Pin] = IM_COL32(255, 255, 255, 255);
    editor_style.Colors[ImNodesCol_PinHovered] = IM_COL32(150, 150, 255, 255);


    // Initialize ImGui for SDL2
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool quit = false;


    while (!quit) {
        
        if (started == true){
            started = false;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {quit = true;}     
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DELETE) {
                    delete_selected();
                }
                if (event.key.keysym.sym == SDLK_c) {
                    copy_nodes();
                }
                if (event.key.keysym.sym == SDLK_v) {
                    paste_nodes();
                }
                if (event.key.keysym.sym == SDLK_r) {
                    started = true;
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