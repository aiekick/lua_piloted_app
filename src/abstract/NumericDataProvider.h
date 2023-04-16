#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "DataProvider.h"

template <typename T>
class NumericDataProvider : public DataProvider<T>
{
private:
    std::unordered_map<std::string, T> m_Values;

public:
    virtual ~NumericDataProvider() = default;

    void setValue(const std::string& name, const T& value)
    {
        m_Values[name] = value;
        notify(name, value);
    }

    T getValue(const std::string& name) const
    {
        return m_Values.at(name);
    }
};
