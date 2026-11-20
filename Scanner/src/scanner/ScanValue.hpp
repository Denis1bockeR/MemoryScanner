#pragma once

#include "../defines.h"

namespace core
{
    struct ScanValue
    {
        ValueType m_type = ValueType::Int32;
        ValuePayload m_payload;

        template <typename T>
        static ScanValue makeNumber(ValueType t, T v)
        {
            ScanValue s;
            s.m_type = t;
            if (t == ValueType::Float || t == ValueType::Double)
                s.m_payload = static_cast<double>(v);
            else
                s.m_payload = static_cast<int64_t>(v);
            return s;
        }

        static ScanValue makeString(std::string v)
        {
            ScanValue s;
            s.m_type = ValueType::String;
            s.m_payload = std::move(v);
            return s;
        }

        static ScanValue makeBytes(std::vector<uint8_t> v)
        {
            ScanValue s;
            s.m_type = ValueType::Bytes;
            s.m_payload = std::move(v);
            return s;
        }

        std::vector<uint8_t> toBytes() const;
    };

    enum class ScanCompare
    {
        Equal,
        NotEqual,
        Greater,
        Less,
        GreaterOrEqual,
        LessOrEqual,
        Changed,
        Unchanged,
        Increased,
        Decreased,
        Unknown
    };

    inline const char* scanCompareName(ScanCompare c)
    {
        switch (c)
        {
        case ScanCompare::Equal:          return "equal";
        case ScanCompare::NotEqual:       return "not equal";
        case ScanCompare::Greater:        return "greater";
        case ScanCompare::Less:           return "less";
        case ScanCompare::GreaterOrEqual: return ">=";
        case ScanCompare::LessOrEqual:    return "<=";
        case ScanCompare::Changed:        return "changed";
        case ScanCompare::Unchanged:      return "unchanged";
        case ScanCompare::Increased:      return "increased";
        case ScanCompare::Decreased:      return "decreased";
        case ScanCompare::Unknown:        return "unknown";
        }
        return "";
    }

    inline std::vector<uint8_t> ScanValue::toBytes() const
    {
        std::vector<uint8_t> out;
        switch (m_type) {
        case ValueType::Int8:
        {
            out.resize(1);
            out[0] = static_cast<uint8_t>(std::get<int64_t>(m_payload));
            break;
        }
        case ValueType::Int16: {
            int16_t v = static_cast<int16_t>(std::get<int64_t>(m_payload));
            out.resize(sizeof(v));
            std::memcpy(out.data(), &v, sizeof(v));
            break;
        }
        case ValueType::Int32:
        {
            int32_t v = static_cast<int32_t>(std::get<int64_t>(payload));
            out.resize(sizeof(v));
            std::memcpy(out.data(), &v, sizeof(v));
            break;
        }
        case ValueType::Int64:
        {
            int64_t v = std::get<int64_t>(payload);
            out.resize(sizeof(v));
            std::memcpy(out.data(), &v, sizeof(v));
            break;
        }
        case ValueType::Float:
        {
            float v = static_cast<float>(std::get<double>(m_payload));
            out.resize(sizeof(v));
            std::memcpy(out.data(), &v, sizeof(v));
            break;
        }
        case ValueType::Double:
        {
            double v = std::get<double>(m_payload);
            out.resize(sizeof(v));
            std::memcpy(out.data(), &v, sizeof(v));
            break;
        }
        case ValueType::String:
        {
            const auto& s = std::get<std::string>(m_payload);
            out.assign(s.begin(), s.end());
            break;
        }
        case ValueType::Bytes:
            out = std::get<std::vector<uint8_t>>(m_payload);
            break;
        }
        return out;
    }
}