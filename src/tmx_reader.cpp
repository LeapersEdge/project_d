#include "tmx_reader.hpp"
#include <functional>

//-----------------------------------------------------------------
// TMX_LAYER_BUFFER
//-----------------------------------------------------------------

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
    
    DebugLogInfo("TMX_LAYER_BUFFER: Finished constructing");
}


const std::vector<std::vector<unsigned long>>& Tmx_Layer_Buffer::Get_Layer_Buffer_Sprite_IDs()
{
    return tile_sprite_IDs;
}

//-----------------------------------------------------------------
// TMX_LAYER_BUFFER
//-----------------------------------------------------------------
// TMX_READER
//-----------------------------------------------------------------

Tmx_Reader::Tmx_Reader(std::string tmx_path)
{   
    DebugLogInfo("TMX_READER: started initializing tilemap at `" + tmx_path + "`");

    std::ifstream in(tmx_path);
    if (in.bad())
    {
        DebugLogError("TMX_READER: FATAL, file at path:`" + tmx_path + "` is bad, possibly corrupted!!!");
        DebugLogError("TMX_READER: ABORTING CONSTRUCTOR");
        successfully_loaded = false;
        return;
    }

    if (in.fail())
    {
        DebugLogError("TMX_READER: Fila at path `" + tmx_path + "` could not be openend. Locked or file doesn't exist as location.");
        DebugLogError("TMX_READER: ABORTING CONSTRUCTOR");
        successfully_loaded = false;
        return;
    }

    std::string line;
    while (!in.eof())
    {
        std::getline(in, line);

        // if the line contains "map version" than it is the map descriptor line and it should be processed
        if (line.find("map version") != std::string::npos)
        {
            DebugLogInfo("TMX_READER: found tilemap descriptor <map>");
            bool success = Parse_Map_Descriptor(line); 
            if (!success)
            {
                successfully_loaded = false;
                DebugLogError("TMX_READER: FATAL, map descriptor has been read wrong or (witdh, height, tilewidth, tileheight) contains 0.");
            }
        }

        // if the line contains "tileset", than it is the tileset info line and it should be processed
        if (line.find("tileset") != std::string::npos)
        {
            DebugLogInfo("TMX_READER: found tileset descriptor <tileset>");
            // TODO: TSX class, from this function needs to read file path, and with that file path, from the TSX class it needs to load it, also
            // after its loaded, it needs to be stored into TSX container class here, along side with the descriptors with from which index the tiles start
            // (vector with TSXs hasnt been created in the TMX_READER, that needs to be done)

            // +8 for "source=\"", +6 for "../../"
            size_t source_pos = line.find("source=\"") + 8 + 6;
            size_t firstgid_pos = line.find("firstgid=\"") + 10;
            std::string line_clone = line;
            line.erase(0, source_pos);
            line.erase(line.size() - 3); // removing "/>
            line_clone.erase(0, firstgid_pos);
            line_clone.erase(source_pos - 2); // removing " before source
            tsx_datas.push_back({std::stoul(line_clone), Tsx_Data(RESOURCES_PATH + line)});
        }

        // if the line contains "data encoding=\"csv\"", than it is the tmx layer data and it should be processed
        if (line.find("data encoding=\"csv\"") != std::string::npos)
        {
            map_layers.push_back(Tmx_Layer_Buffer(in));            
        }

        // otherwise, the contents of the line dont matter
    }
}

bool Tmx_Reader::Draw(raylib::Vector2 origin)
{
    // check if tmx reader is valid (tmx was succesfully loaded and all files were found)
    if (!successfully_loaded)
        return false;

    // draw
    // 1st, drawing layer by layer, getting all the tile IDs and than searching for them though TSX data to draw the tile
    raylib::Vector2 pos = origin;
    unsigned long tile_sprite_ID;
    bool found_element = false;
    for (int layer = map_layers.size() - 1; layer >= 0; layer--)
    { 
        // going by rows of the layer map
        const std::vector<std::vector<unsigned long>>& layer_sprite_IDs = map_layers[layer].Get_Layer_Buffer_Sprite_IDs();
        for (int y = 0; y < layer_sprite_IDs.size(); y++)
        {
            // going by each element in row of layer map
            for (int x = 0; x < layer_sprite_IDs[0].size(); x++)
            {
                // getting the row element of layer map, finding to which TSX the element belogs, transforming it to relative index, and finally drawing it than 
                // OPTIMISE OPTIMIZE: binary search (if very large, also need to test performance bag before that because caching might be screwed)
                tile_sprite_ID = layer_sprite_IDs[y][x];
                found_element = false;
                for (int i = tsx_datas.size() - 1; !found_element && i >= 0; i--)
                { 
                    unsigned long index = tsx_datas[i].first;
                    if (tile_sprite_ID >= index)
                    {
                        pos.x = origin.x + (x * tmx_tilewidth);
                        pos.y = origin.y + (y * tmx_tileheight);
                        tsx_datas[i].second.Draw_Tile(pos, tile_sprite_ID - index);
                        found_element = true;
                    }
                }
            }
        }
    }

    return true;
}

bool Tmx_Reader::Is_Valid()
{
    return successfully_loaded;
}

// this function is way slower than it should be, but its ok, it runs only when Tmx_Reader is initialized which is once per scene
bool Tmx_Reader::Parse_Map_Descriptor(std::string line)
{
    std::vector<std::string> all_words;
    
    // first, we split line by space and put into all_words
    char c;
    std::string temp_word = "";
    for (int i = 0; i < line.size(); i++)
    {
        c = line[i];

        if (c == ' ')
        {
            all_words.push_back(temp_word);
            temp_word = "";
        }
        else
        {
            temp_word += c;
        }

        // we dont account for last word because it is nextobjectid and i dont care about that yet
    }

    // second, we find the words we want and slice of characters of the actual words (width, height, etc.) & " characters
    for (int i = 0; i < all_words.size(); i++)
    {
        std::string word = all_words[i];
        
        // in all of these if cases we use continue because if word contains "tilewidth", it also contains "width", thus we need to ensure both dont run but only 1 does

        if (word.find("tilewidth") != std::string::npos)
        {
            word.erase(0, 11);
            word.erase(word.size() - 1);
            
            tmx_tilewidth = std::stoul(word);
            continue;
        }

        if (word.find("tileheight") != std::string::npos)
        {
            word.erase(0, 12);
            word.erase(word.size() - 1);
            
            tmx_tileheight = std::stoul(word);
            continue;
        }

        if (word.find("width") != std::string::npos)
        {
            word.erase(0, 7);
            word.erase(word.size() - 1);
           
            tmx_width = std::stoul(word);
            continue;
        }
        
        if (word.find("height") != std::string::npos)
        {
            word.erase(0, 8);
            word.erase(word.size() - 1);
            
            tmx_height = std::stoul(word);
            continue;
        } 
    }

    // third, we check if we successfully loaded the map (even if it was successfully loaded but any of the variables are 0, the map is unusable and should abort itself)

    if (tmx_height == 0 || tmx_width == 0 || tmx_tilewidth == 0 || tmx_tileheight == 0)
        return false;

    return true;
}

//-----------------------------------------------------------------
// TMX_READER
//-----------------------------------------------------------------
