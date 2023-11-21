#include "tmx_reader.hpp"

Tmx_Reader::Tmx_Reader(std::string tmx_path)
{     
    
}

bool Tmx_Reader::Draw(raylib::Vector2 origin)
{
    // check if tmx reader is valid (tmx was succesfully loaded and all files were found)
    if (!Is_Valid())
        return false;

    // draw

    return true;
}

bool Tmx_Reader::Is_Valid()
{
    return successfully_loaded;
}
