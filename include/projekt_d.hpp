#include "raylib-cpp.hpp"
#include "raygui/raygui.h"

#ifdef DEBUG
    #include "debug.hpp"

    #define DebugLog(message) Debug::Log(message)
    #define DebugLogInfo(message) Debug::Log_Info(message)
    #define DebugLogWarning(message) Debug::Log_Warning(message)
    #define DebugLogError(message) Debug::Log_Error(mesasge)
#else
    #define DebugLog(message) 
    #define DebugLogInfo(message) 
    #define DebugLogWarning(message) 
    #define DebugLogError(message) 
#endif

#include "game.hpp"

