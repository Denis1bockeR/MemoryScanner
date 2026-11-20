#pragma once

#include <cstdint>
#include <string>
#include <variant>  
#include <vector>

#include <Windows.h>

#ifdef _WIN32
using pid_t = uint32_t;
#else
#include <sys/types.h>
#endif

namespace core
{
    struct ProcessInfo {
        pid_t pid = 0;
        pid_t parentPid = 0;
        std::string name;
        std::string path;
        bool is64bit = false;   
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

    enum class ValueType 
    {
        Int8,
        Int16,
        Int32,
        Int64,
        Float,
        Double,
        String,
        Bytes
    };

    inline size_t valueTypeSize(ValueType t) 
    {
        switch (t) 
        {
        case ValueType::Int8:   return 1;
        case ValueType::Int16:  return 2;
        case ValueType::Int32:  return 4;
        case ValueType::Int64:  return 8;
        case ValueType::Float:  return 4;
        case ValueType::Double: return 8;
        case ValueType::String: return 0;
        case ValueType::Bytes:  return 0;
        }
        return 0;
    }

    inline const char* valueTypeName(ValueType t) 
    {
        switch (t) 
        {
        case ValueType::Int8:   return "int8";
        case ValueType::Int16:  return "int16";
        case ValueType::Int32:  return "int32";
        case ValueType::Int64:  return "int64";
        case ValueType::Float:  return "float";
        case ValueType::Double: return "double";
        case ValueType::String: return "string";
        case ValueType::Bytes:  return "bytes";
        }
        return "";
    }

    using ValuePayload = std::variant<
        std::monostate,
        int64_t,
        double,
        std::string,
        std::vector<uint8_t>
    >;

    inline bool isProcess64bit(HANDLE hProc)
    {
        int wow64 = 0;
        if (!::IsWow64Process(hProc, &wow64))
            return sizeof(void*) == 8;
        return (sizeof(void*) == 8) && !wow64;
    }
}