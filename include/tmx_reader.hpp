#pragma once

#include <string>
#include <vector>
#include "Vector2.hpp"
#include <fstream>
#include "debug.hpp"
#include "tsx_data.hpp"
#include <utility>

class Tmx_Layer_Buffer
{
public:
    Tmx_Layer_Buffer(std::ifstream& in);

    const std::vector<std::vector<unsigned long>>& Get_Layer_Buffer_Sprite_IDs();
private:
    // POSSIBLE FAILIURE: max of unsigned long is 4294967295, if tile sprite ID exceeds that limit, it will break
    // POSSIBLE FAILIURE FIX: unsigned long -> unsigned long long (OCCURANCES:tmx_reader.hpp & tmx_reader.cpp NOTE: std::stoul -> std::stoull in Tmx_Layer_Buffer constructor as well)
    // vector<vector<>> because is 2D table if tile sprite IDs 
    std::vector<std::vector<unsigned long>> tile_sprite_IDs;
};

class Tmx_Reader
{
public:
    Tmx_Reader(std::string tmx_path);

    bool Draw(raylib::Vector2 top_left_origin);
    bool Is_Valid();
private:
    bool successfully_loaded = true;
    
    bool Parse_Map_Descriptor(std::string line);

    std::vector<Tmx_Layer_Buffer> map_layers;
    unsigned long tmx_width = 0;
    unsigned long tmx_height = 0;
    unsigned long tmx_tilewidth = 0;
    unsigned long tmx_tileheight = 0;

    // unsigned long is starting ID, tsx_data is tsx_data
    // starting ID is relative to tmx data so it is stored here and not inside tsx
    std::vector<std::pair<unsigned long, Tsx_Data>> tsx_datas;
};
