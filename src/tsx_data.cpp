#include "tsx_data.hpp"

Tsx_Data::Tsx_Data(std::string tsx_path)
    :
    path(tsx_path)
{
    // literally copy paste from TMX_READER up until inside while loop, if you want my comments, refer to that
    DebugLogInfo("TSX_DATA: started initializing tilemap at `" + tsx_path + "`");

    std::ifstream in(tsx_path);
    if (in.bad())
    {
        DebugLogError("TSX_DATA: FATAL, file at path:`" + tsx_path + "` is bad, possibly corrupted!!!");
        DebugLogError("TSX_DATA: ABORTING CONSTRUCTOR");
        successfully_loaded = false;
        return;
    }

    if (in.fail())
    {
        DebugLogError("TSX_DATA: Fila at path `" + tsx_path + "` could not be openend. Locked or file doesn't exist as location.");
        DebugLogError("TSX_DATA: ABORTING CONSTRUCTOR");
        successfully_loaded = false;
        return;
    }

    std::string line;
    while (!in.eof())
    {
        std::getline(in, line);
        
        if (line.find("<tileset") != std::string::npos)
        {
            std::string clone = line;
            
            // the order im cutting the strings is because it is always formatted in such a set order, tilewidth tileheight tilecount columns

            line.erase(0, line.find("tilewidth=") + 11);
            line.erase(line.find("tileheight=") - 2);
            tilewidth = std::stoul(line);

            line = clone;
            line.erase(0, line.find("tileheight=") + 12);
            line.erase(line.find("tilecount=") - 2);
            tileheight = std::stoul(line);

            line = clone;
            line.erase(0, line.find("tilecount=") + 11);
            line.erase(line.find("columns=") - 2);
            tilecount = std::stoul(line);

            line = clone;
            line.erase(0, line.find("columns=") + 9);
            line.erase(line.size() - 2);
            columns = std::stoul(line);
        }

        if (line.find("<image") != std::string::npos)
        {
            size_t src_index = line.find("source=") + 11;
            line.erase(0, src_index);

            size_t src_end_index = line.find("png\"") + 3;
            line.erase(src_end_index);
            
            line = "tilesets/" + line;
            DebugLogInfo("TSX_DATA: loaded tsx, tileweigth:" + std::to_string(tilewidth) + " | tileheight:" + std::to_string(tileheight) + " | tilecount:" + std::to_string(tilecount) + " | columns:" + std::to_string(columns) + RESOURCES_PATH + line);
            texture.Load(RESOURCES_PATH + line);
        }
    }    

    if (tilewidth == 0 | tileheight == 0 | tilecount == 0 | columns == 0)
    {
        successfully_loaded = false;
        DebugLogError("TSX_DATA: FAILIURE, one of the atributes from <tileset> is == 0, illegal behaveour, aborting functionality of: " + tsx_path);
    }
}

bool Tsx_Data::Draw_Tile(raylib::Vector2 position, unsigned int relative_tile_ID)
{
    if (!successfully_loaded)
        return false;
    
    unsigned int pic_start_x = tilewidth * (relative_tile_ID % columns);
    unsigned int pic_start_y = tileheight * (relative_tile_ID / (tilecount / columns));

    std::cout << pic_start_x << ' ' << pic_start_y << ' ' << path << std::endl;

    texture.Draw(raylib::Rectangle(pic_start_x, pic_start_y, tilewidth, tileheight), position);

    return true; 
}

bool Tsx_Data::Is_Valid()
{
    return successfully_loaded;
}
