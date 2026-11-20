#pragma once

#include <cstdint>
#include <string>

#ifdef _WIN32
using pid_t = uint32_t;
#else
#include <sys/types.h>
#endif

namespace core
{
    struct ProcessInfo {
        pid_t    pid = 0;
        pid_t    parentPid = 0;
        std::string name;
        std::string path;
        bool        is64bit = false;
    };

    struct MemoryRegion
    {
        uintptr_t base = 0;
        size_t    size = 0;
        uint32_t  protect = 0;
        uint32_t  state = 0;   // MEM_COMMIT / MEM_FREE / MEM_RESERVE
        uint32_t  type = 0;   // MEM_IMAGE / MEM_MAPPED / MEM_PRIVATE
        bool      readable = false;
        bool      writable = false;
        bool      executable = false;
    };
}