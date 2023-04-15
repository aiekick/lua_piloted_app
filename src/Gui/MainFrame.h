/*
 * Copyright 2022-2023 Stephane Cuillerdier (aka Aiekick)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <glad/glad.h>
#include <imgui/imgui.h>

#include <Core/LuaVM.h>

#include <functional>
#include <string>
#include <vector>
#include <map>

struct GLFWwindow;
class MainFrame
{
private:
	GLFWwindow* m_Window_Ptr = nullptr;
	
	ImVec2 m_DisplayPos = ImVec2(0, 0); // viewport
	ImVec2 m_DisplaySize = ImVec2(1280, 720);

	bool m_ShowImGui = false;				// show ImGui win
	bool m_ShowMetric = false;				// show metrics

	LuaVM m_LuaVM;

public:
	bool init();
	void unit();

	void display(ImVec2 vPos, ImVec2 vSize);

	GLFWwindow* GetGLFWwindow() { return m_Window_Ptr; }

private: // imgui pane / dialogs
	void DrawMainMenuBar();
	void DisplayDialogsAndPopups();

private:
	void SetAppTitle();

public: // singleton
	static MainFrame *Instance(GLFWwindow *vWin = 0)
	{
		static MainFrame s_instance(vWin);
		return &s_instance;
	}

protected:
	MainFrame(GLFWwindow *vWin);
	MainFrame(const MainFrame&) = default;
	MainFrame& operator =(const MainFrame&) { return *this; };
	~MainFrame();
};

