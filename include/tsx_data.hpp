#pragma once

#include <string>
#include "raylib-cpp.hpp"
#include "debug.hpp"
#include <fstream>

class Tsx_Data
{
public:
    Tsx_Data(std::string file_path);

    bool Draw_Tile(raylib::Vector2 position, unsigned int relative_tile_ID);
    bool Is_Valid();
private:
    std::string path;
    bool successfully_loaded = true;

    raylib::Texture2D texture;
    unsigned long tilewidth = 0;
    unsigned long tileheight = 0;
    unsigned long tilecount = 0;
    unsigned long columns = 0;
};
