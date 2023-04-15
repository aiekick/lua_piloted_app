#pragma once

#include <string>

class ProjectBuilder
{
public:
	void LoadProjectFromFile(const std::string& vProjectFilePathName);
	void LoadProjectFromString(const std::string& vProjectstring);

private:
	void InitLuaVM();
	void UnitLuaVM();
};