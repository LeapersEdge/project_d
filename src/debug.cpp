#include "global_defines.hpp"

#ifdef DEBUG
    #include "debug.hpp"
    std::mutex Debug::mutex;
#endif
