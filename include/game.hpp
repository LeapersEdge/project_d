#pragma once

#include "tmx_reader.hpp"
#include "debug.hpp"

class Game
{
public:
    Game();

    void Init();
    void Update();
    void Render();
    void Post_Update();
private:
    Tmx_Reader tmx;
    raylib::Texture2D tex;
};
