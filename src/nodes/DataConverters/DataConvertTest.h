#pragma once

#include <abstract/LuaNode.h>

class DataConvertTest : public LuaNode
{
public:
	~DataConvertTest() override;
	bool buildNode(const sol::table& vTable) override;
};