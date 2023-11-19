#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <imgui_node_editor.h>
#include <iostream>
#include "editor.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


int main( int argc, char* args[] ) {


    editor::Config editor_config;
    editor_config.SettingsFile = "editor_config.json";
    auto editor_context = editor::CreateEditor(&editor_config);

    SDL_Window* window = NULL;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }

    else {
        //Create window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
        
        SDL_GLContext glcontext = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, glcontext);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.IniFilename = "lambda_layout";

        setup_theme();

        ImGui_ImplSDL2_InitForOpenGL(window,glcontext);
        ImGui_ImplOpenGL3_Init("#version 130");

        if( window == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }

        SDL_Event event; 
        bool quit = false;

        while( quit == false ){

            while( SDL_PollEvent( &event ) ){
                ImGui_ImplSDL2_ProcessEvent(&event);
                if ( event.type == SDL_QUIT ) {
                    quit = true;
                }                
            }                

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);
            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(0,0));
            ImGui::SetNextWindowSize(io.DisplaySize);
            ImGui::Begin("workspace",nullptr,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
            ImGuiID dockspace_id = ImGui::GetID("workspace");
            ImGui::DockSpace(dockspace_id);

            ImGui::Begin("editor");

            editor::SetCurrentEditor(editor_context);
            run_editor();
            editor::SetCurrentEditor(nullptr);

            ImGui::End();
            ImGui::End();

            glViewport(0,0,(int)io.DisplaySize.x,(int)io.DisplaySize.y);
            glClearColor(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui::Render();       
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
                              
        }
    }

    editor::DestroyEditor(editor_context);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0; 
}