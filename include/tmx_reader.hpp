#pragma once

#include <string>
#include <vector>
#include "Vector2.hpp"

class Tmx_Reader
{
public:
    Tmx_Reader(std::string tmx_path);

    bool Draw(raylib::Vector2 top_left_origin);
private:
    bool successfully_loaded;
    bool Is_Valid();
};
