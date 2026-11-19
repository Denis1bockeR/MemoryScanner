#pragma once

#include <vector>
#include "defines.h"

namespace core
{
    class IProcessMemory
    {
    public:
        virtual ~IProcessMemory() = default;

        virtual bool attach(pid_t pid) = 0;
        virtual void detach() = 0;
        virtual bool isAttached() const = 0;
        virtual pid_t pid() const = 0;

        virtual bool read(uintptr_t address, void* buffer, size_t size) = 0;
        virtual bool write(uintptr_t address, const void* buffer, size_t size) = 0;

        virtual std::vector<MemoryRegion> queryRegions() = 0;
        virtual size_t pointerSize() const = 0;  // 4 or 8
        virtual bool isTarget64bit() const = 0;
    };
}   