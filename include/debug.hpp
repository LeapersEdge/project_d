#pragma once

#include "global_defines.hpp"

#ifdef DEBUG
    #define DebugLog(message) Debug::Log(message)
    #define DebugLogInfo(message) Debug::Log_Info(message)
    #define DebugLogWarning(message) Debug::Log_Warning(message)
    #define DebugLogError(message) Debug::Log_Error(message)
#else
    #define DebugLog(message) 
    #define DebugLogInfo(message) 
    #define DebugLogWarning(message) 
    #define DebugLogError(message) 
#endif

#ifdef DEBUG
    #include <fstream>
    #include <string>
    #include <mutex>
    #include <chrono>
    #include <ctime>

    // a singleton structure
    class Debug
    {
    private:
        static void Generic_Log(std::string& message, std::string log_type)
        {
            // getting the timestamp ready
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            char timestamp[9];
            std::strftime(timestamp, sizeof(timestamp), "%H:%M:%S", std::localtime(&now_c));

            // mutex lock, ensures that no thread can access the rest of the function until it is released (lock_guard releases automatically with destructor)
            std::lock_guard<std::mutex> lock(mutex);

            std::ofstream out("debug_log.txt", std::ios_base::app);
            out << log_type << "[" << timestamp << "] " << message << "\n";
            out.flush(); 
        }

    public:
        static void Log(std::string message)
        {
            Generic_Log(message, "[LOG    ]");   
        }

        static void Log_Info(std::string message)
        {
            Generic_Log(message, "[INFO   ]");   
        }

        static void Log_Warning(std::string message)
        {
            Generic_Log(message, "[WARNING]");   
        }

        static void Log_Error(std::string message)
        { 
            Generic_Log(message, "!!![  ERROR  ]");   
        }

    private:
        Debug() {std::ofstream("debug_log.txt", std::ios::trunc);}
        Debug(const Debug&) = delete;
        Debug& operator=(const Debug&) = delete;

        static std::mutex mutex;
    };

#endif
