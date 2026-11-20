#pragma once

#include "../defines.h"

namespace core
{
    struct ScanResult 
    {
        uintptr_t address = 0;
        std::vector<uint8_t> previousValue;
        bool readable = true;

        ScanResult() = default;
        ScanResult(uintptr_t a, std::vector<uint8_t> prev)
            : address(a), previousValue(std::move(prev)) {}

        bool asInt64(int64_t& out) const
        {
            if (previousValue.empty() || previousValue.size() > 8) return false;
            int64_t v = 0;
            std::memcpy(&v, previousValue.data(), previousValue.size());
            out = v;
            return true;
        }

        bool asDouble(double& out) const
        {
            if (previousValue.empty() || previousValue.size() > 8) return false;
            int64_t v = 0;
            std::memcpy(&v, previousValue.data(), previousValue.size());
            out = v;
            return true;
        }

        bool asFloat(float& out) const
        {
            if (previousValue.size() != sizeof(float)) return false;
            std::memcpy(&out, previousValue.data(), sizeof(float));
            return true;
        }
    };
}