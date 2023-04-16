#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <sol/sol.hpp>
#include <abstract/LuaNode.h>

class NodeFactory
{
public:
    static std::shared_ptr<NodeFactory> create() {
        return std::make_shared<NodeFactory>();
    }

public:
    template<typename T>
    void register_node(const std::string& typeName) {
        m_NodeRegistry[typeName] = []() {
            return std::make_shared<T>();
        };
    }

    std::shared_ptr<LuaNode> create(const std::string& vNodeName, const sol::table& vNodeDatas) {
        auto it = m_NodeRegistry.find(vNodeName);
        if (it != m_NodeRegistry.end()) {
            auto ptr = it->second();
            if (ptr && ptr->buildNode(vNodeDatas)) {
                return ptr;
            }
            ptr.reset();
        }
        return nullptr;
    }

    void clear() {
        m_NodeRegistry.clear();
    }

private:
    std::unordered_map<std::string, std::function<std::shared_ptr<LuaNode>()>> m_NodeRegistry;
};