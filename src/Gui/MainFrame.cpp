// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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

#include "MainFrame.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <ImGuiFileDialog/ImGuiFileDialog.h>
#include <Contrib/ImWidgets/ImWidgets.h>

#include <Contrib/FontIcons/CustomFontToolBar.h>

#include <imgui/imgui_internal.h>

#include <Lua/LuaEngine.h>
#include <Headers/Globals.h>

#include <Panes/Manager/LayoutManager.h>
#include <Panes/ToolPane.h>

#define WIDGET_ID_MAGIC_NUMBER 4577

 //////////////////////////////////////////////////////////////////////////////
 //// UTILS ///////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////////

std::string toStr(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char TempBuffer[3072 + 1];//3072 = 1024 * 3
	const int w = vsnprintf(TempBuffer, 3072, fmt, args);
	va_end(args);
	if (w)
	{
		return std::string(TempBuffer, (size_t)w);
	}
	return std::string();
}

//////////////////////////////////////////////////////////////////////////////
//// CTOR ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame(GLFWwindow *vWin)
{
	m_Window_Ptr = vWin;
}

MainFrame::~MainFrame() = default;

//////////////////////////////////////////////////////////////////////////////
//// INIT / UNIT /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

bool MainFrame::init()
{
	SetAppTitle(); 
	
	LayoutManager::Instance()->Init(ICON_NDPTB_DESKTOP_MAC " Layouts", "Default Layout");

	LayoutManager::Instance()->SetPaneDisposalSize(PaneDisposal::LEFT, 200.0f);
	LayoutManager::Instance()->SetPaneDisposalSize(PaneDisposal::RIGHT, 500.0f);
	LayoutManager::Instance()->SetPaneDisposalSize(PaneDisposal::BOTTOM, 200.0f);

	LayoutManager::Instance()->AddPane(ToolPane::Instance(), ICON_NDP2_CUBE_SCAN " Tool", "", PaneDisposal::LEFT, true, true);
	
	bool res = true;

	res &= LayoutManager::Instance()->InitPanes();
	res &= LuaEngine::Instance()->Init();
	
	return res;
}

void MainFrame::unit()
{
	LuaEngine::Instance()->Unit();
	LayoutManager::Instance()->UnitPanes();
}

//////////////////////////////////////////////////////////////////////////////
//// DISPLAY /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void MainFrame::display(ImVec2 vPos, ImVec2 vSize)
{
	m_DisplayPos = vPos;
	m_DisplaySize = vSize;

	const auto context = ImGui::GetCurrentContext();
	if (context)
	{
		ImGui::CustomStyle::Instance()->pushId = 4577;

		if (ImGui::BeginMainMenuBar())
		{
			DrawMainMenuBar();

			// ImGui Infos
			const auto label = toStr("Dear ImGui %s (Docking)", ImGui::GetVersion());
			const auto size = ImGui::CalcTextSize(label.c_str());
			ImGui::Spacing(ImGui::GetContentRegionAvail().x - size.x - ImGui::GetStyle().FramePadding.x * 2.0f);
			ImGui::Text("%s", label.c_str());

			ImGui::EndMainMenuBar();
		}

		if (ImGui::BeginMainStatusBar())
		{
#ifdef _DEBUG
			// ImGui Infos
			const auto& io = ImGui::GetIO();
			const auto fps = toStr("%.1f ms/frame (%.1f fps)", 1000.0f / io.Framerate, io.Framerate);
			const auto size = ImGui::CalcTextSize(fps.c_str());
			ImGui::Spacing(ImGui::GetContentRegionAvail().x - size.x - ImGui::GetStyle().FramePadding.x * 2.0f);
			ImGui::Text("%s", fps.c_str());
#endif

			ImGui::EndMainStatusBar();
		}

		if (LayoutManager::Instance()->BeginDockSpace(ImGuiDockNodeFlags_PassthruCentralNode))
		{
			LayoutManager::Instance()->EndDockSpace();
		}

		auto* cstyle = ImGui::CustomStyle::Instance();
		cstyle->pushId = LayoutManager::Instance()->DisplayPanes(0U, cstyle->pushId, "");

		DisplayDialogsAndPopups();

		LayoutManager::Instance()->InitAfterFirstDisplay(vSize);

		if (m_ShowImGui)
			ImGui::ShowDemoWindow();
		if (m_ShowMetric)
			ImGui::ShowMetricsWindow(&m_ShowMetric);
	}
}

//////////////////////////////////////////////////////////////////////////////
//// PRIVATE'S ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void MainFrame::DrawMainMenuBar()
{
	/*if (ImGui::BeginMenu(ICON_NDP_DOT_CIRCLE_O " Project"))
	{
		if (ImGui::MenuItem(ICON_NDP2_FILE " New"))
		{
			Action_Menu_NewProject();
		}

		ImGui::EndMenu();
	}*/

	LayoutManager::Instance()->DisplayMenu(m_DisplaySize);
}

void MainFrame::DisplayDialogsAndPopups()
{
	LayoutManager::Instance()->DrawDialogsAndPopups(0U, "");
}

void MainFrame::SetAppTitle()
{
	static char bufTitle[1024] = "";
	snprintf(bufTitle, 1023, "%s %s", LUA_PILOTED_APPNAME, LUA_PILOTED_APP_VERSION_ID);
	glfwSetWindowTitle(m_Window_Ptr, bufTitle);
}
