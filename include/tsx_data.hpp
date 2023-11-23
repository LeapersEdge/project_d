#pragma once

#include <string>
#include "raylib-cpp.hpp"
#include "debug.hpp"

class Tsx_Data
{
public:
    Tsx_Data(std::string file_path);

    bool Draw_Tile(raylib::Vector2 position, unsigned int x_in_tileset, unsigned int y_in_tilemap);
    bool Is_Valid();
private:
    bool successfully_loaded;

    raylib::Texture2D texture;
    unsigned long tilecount;
    unsigned long column;
};
