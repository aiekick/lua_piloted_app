#pragma once

#include <string>

class Graph
{
public:
	void LoadProjectFromFile(const std::string& vProjectFilePathName);
	void LoadProjectFromString(const std::string& vProjectstring);

private:
	void InitLuaVM();
	void UnitLuaVM();
};