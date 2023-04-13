#pragma once

#include <string>
#include <functional>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <array>
#include <memory>

const uint32_t DEFAULT_WIDTH = 1700;
const uint32_t DEFAULT_HEIGHT = 700;
struct GLFWwindow;
class App
{
public:
	App() = default;
	~App() = default;

	bool init(const int& vArgc, char** vArgv);
	void run();
	bool unit();
};
