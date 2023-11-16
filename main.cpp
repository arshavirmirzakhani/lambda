#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <imgui_node_editor.h>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



int main( int argc, char* args[] ) {

    SDL_Window* window = NULL;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }

    else {
        //Create window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        
        SDL_GLContext glcontext = SDL_GL_CreateContext(window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiStyle style = ImGui::GetStyle();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.IniFilename = "lambda_layout";

        ImGui_ImplSDL2_InitForOpenGL(window,glcontext);
        ImGui_ImplOpenGL3_Init("#version 330");

        if( window == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }

        SDL_Event event; 
        bool quit = false;
        while( quit == false ){

            while( SDL_PollEvent( &event ) ){
                if ( event.type == SDL_QUIT ) {
                    quit = true;
                }
            }
                

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);
            ImGui::NewFrame();
                
            ImGui::Begin("hello");
            ImGui::End();

            glViewport(0,0,(int)io.DisplaySize.x,(int)io.DisplaySize.y);
            glClearColor(0.2f,0.2f,0.2f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            SDL_GL_SwapWindow(window);
                
                 
        }
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0; 
}