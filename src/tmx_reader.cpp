#include "tmx_reader.hpp"

//-----------------------------------------------------------------
// TMX_LAYER_BUFFER
//-----------------------------------------------------------------

Tmx_Layer_Buffer::Tmx_Layer_Buffer(std::ifstream& in)
{
    // over here, were going though CSV part of TMX, this is called in contructor of TMX Reader, see Tmx_Reader.hpp
    // we achive filling TMX Layer Buffer by basically parsing CSV to 2D uint32_t table, so this entire constructor will be exactly that
    // end of the CSV is marked by "</data>" (the entire line is exactly just that, there are no extra whitespaces or anything)
    
    DebugLogInfo("TMX_LAYER_BUFFER: Started constructing");    

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

unsigned long Tmx_Layer_Buffer::Get_Tile_ID(unsigned int x, unsigned int y)
{
    return tile_sprite_IDs[y][x];
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
        in >> line;

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
        }

        // if the line contains "data encoding=\"csv\"", than it is the tmx layer data and it should be processed
        if (line.find("data encoding=\"csv\"") != std::string::npos)
        {
            DebugLogInfo("TMX_READER: found tilemap data <data>");
            map_layers.push_back(Tmx_Layer_Buffer(in));            
        }

        // otherwise, the contents of the line dont matter
    }
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
            
            tmx_tilewidth = std::stoul(word);
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
