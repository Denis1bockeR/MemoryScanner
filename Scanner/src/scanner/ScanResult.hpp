#pragma once

#include "../defines.h"

namespace core
{
    struct ScanResult 
    {
        uintptr_t m_address = 0;
        std::vector<uint8_t> m_previousValue;
        bool m_readable = true;

        ScanResult() = default;
        ScanResult(uintptr_t a, std::vector<uint8_t> prev)
            : m_address(a), m_previousValue(std::move(prev)) {}

        bool asInt64(int64_t& out) const
        {
            if (m_previousValue.empty() || m_previousValue.size() > 8) return false;
            int64_t v = 0;
            std::memcpy(&v, m_previousValue.data(), m_previousValue.size());
            out = v;
            return true;
        }

        bool asDouble(double& out) const
        {
            if (m_previousValue.empty() || m_previousValue.size() > 8) return false;
            int64_t v = 0;
            std::memcpy(&v, m_previousValue.data(), m_previousValue.size());
            out = v;
            return true;
        }

        bool asFloat(float& out) const
        {
            if (m_previousValue.size() != sizeof(float)) return false;
            std::memcpy(&out, m_previousValue.data(), sizeof(float));
            return true;
        }
    };
}