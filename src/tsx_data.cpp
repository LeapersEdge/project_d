#include "tsx_data.hpp"

Tsx_Data::Tsx_Data(std::string tsx_path)
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
        
        if (line.find("<image") != std::string::npos)
        {
            DebugLogInfo("TSX_DATA: found image descriptor.");
           
            size_t src_index = line.find("source=") + 11;
            line.erase(0, src_index);

            size_t src_end_index = line.find("png\"");
            line.erase(line.size() - src_end_index + 2);
            
            line = "tilesets/" + line;
            DebugLogInfo("TSX_DATA: Trying to load texture at: " RESOURCES_PATH + line);
            texture.Load(RESOURCES_PATH + line);
        }
    }     
}

bool Tsx_Data::Is_Valid()
{
    return successfully_loaded;
}
