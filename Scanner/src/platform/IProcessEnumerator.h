#pragma once

#include <vector>
#include <memory>

#include "defines.h"

namespace core
{
    class IProcessEnumerator 
    {
    public:
        virtual ~IProcessEnumerator() = default;

        virtual std::vector<ProcessInfo> list() = 0;
        virtual bool queryByPid(pid_t pid, ProcessInfo& out) = 0;
        static std::unique_ptr<IProcessEnumerator> create();
    };

}