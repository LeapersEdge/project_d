#include "tmx_reader.hpp"

Tmx_Layer_Buffer::Tmx_Layer_Buffer(std::ifstream& in)
{
    // over here, were going though CSV part of TMX, this is called in contructor of TMX Reader, see Tmx_Reader.hpp
    // we achive filling TMX Layer Buffer by basically parsing CSV to 2D uint32_t table, so this entire constructor will be exactly that
    // end of the CSV is marked by "</data>" (the entire line is exactly just that, there are no extra whitespaces or anything)

    std::string line;
    std::getline(in, line);

    std::string string_buffer;
    char c;
  
    // parsing CSV to 2D unsigned long table --------------------------------------

    while (line != "</data>")
    {
        std::vector<unsigned long> layer_line;

    
        for (int i = 0; i < line.size(); i++)
        {
            c = line[i];

            // if inspecting character is ',' or '\n' than we have the number fully read and need to store it in layer_line
            // string_buffer != "" because there can be a '\n' just after ',' 
            if ((c == ',' || c == '\n') && string_buffer != "")
            {
                layer_line.push_back( std::stoul(string_buffer) );
                string_buffer = "";
            }
            else
            {
                string_buffer += c;
            }
        }
        
        tile_sprite_IDs.push_back(layer_line);
        std::getline(in, line);
    }

    // end of parsing CSV to 2D unsigned long table --------------------------------
}

unsigned long Tmx_Layer_Buffer::Get_Tile_ID(unsigned int x, unsigned int y)
{
    return tile_sprite_IDs[y][x];
}

Tmx_Reader::Tmx_Reader(std::string tmx_path)
{     
    std::ifstream in(tmx_path);    
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
