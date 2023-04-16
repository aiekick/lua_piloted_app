#pragma once

#include <vector>
#include <string>

#include <interfaces/iDataConsumer.h>

template <typename T>
class DataProvider
{
private:
    std::vector<iDataConsumer<T>*> m_Consumers;

public:
    virtual ~DataProvider() = default;

    void attach(iDataConsumer<T>* vConsumer)
    {
        m_Consumers.push_back(consumer);
    }

    void detach(iDataConsumer<T>* vConsumer)
    {
        m_Consumers.erase(std::remove(m_Consumers.begin(), m_Consumers.end(), consumer), m_Consumers.end());
    }

    void notify(const std::string& vName, const T& vValue)
    {
        for (auto consumer : m_Consumers)
        {
            consumer->update(vName, vValue);
        }
    }
};
