#include "projekt_d.hpp"

int main() {
    int screenWidth = 1280;
    int screenHeight = 720; 
    //int screenWidth = GetMonitorWidth(0);
    //int screenHeight = GetMonitorHeight(0);


    raylib::Window window(screenWidth, screenHeight, "raylib-cpp - basic window");

    SetTargetFPS(60);

    Game game;

    game.Init();

    while (!window.ShouldClose())
    {
        game.Update();

        BeginDrawing();
        
        window.ClearBackground(RAYWHITE);
        game.Render();
        game.Post_Update();
        
        EndDrawing();
    }

    // UnloadTexture() and CloseWindow() are called automatically.

    return 0;
}
