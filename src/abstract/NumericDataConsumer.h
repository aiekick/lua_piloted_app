#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <interfaces/iDataConsumer.h>

// La classe NumericDataConsumer est une implémentation concrète d'un DataConsumer pour des valeurs numériques
template <typename T>
class NumericDataConsumer : public iDataConsumer<T> 
{
private:
    std::string m_Name;
    T m_Value;

public:
    NumericDataConsumer(const std::string& name, const T& initialValue) : m_Name(name), m_Value(initialValue) {}

    void update(const std::string& name, const T& value) override 
    {
        if (name == m_Name) 
        {
            std::cout << "Value changed for " << m_Name << ": " << value << std::endl;
            m_Value = value;
        }
    }

    const std::string& getName() const 
    {
        return m_Name;
    }

    const T& getValue() const
    {
        return m_Value;
    }
};
