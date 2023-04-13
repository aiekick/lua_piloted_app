/*
Copyright 2022-2023 Stephane Cuillerdier (aka aiekick)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ToolPane.h"
#include <Gui/MainFrame.h>
#include <Contrib/ImWidgets/ImWidgets.h>
#include <imgui/imgui_internal.h>
#include <Panes/Manager/LayoutManager.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>
#include <cinttypes> // printf zu

#include <Lua/LuaEngine.h>

static int SourcePane_WidgetId = 0;

///////////////////////////////////////////////////////////////////////////////////
//// IMGUI PANE ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void ToolPane::Clear()
{
	
}

bool ToolPane::Init()
{
	return true;
}

void ToolPane::Unit()
{

}

int ToolPane::DrawPanes(const uint32_t& /*vCurrentFrame*/, const int& vWidgetId, const std::string& /*vvUserDatas*/, PaneFlag& vInOutPaneShown)
{
	SourcePane_WidgetId = vWidgetId;

	if (vInOutPaneShown & m_PaneFlag)
	{
		static ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (ImGui::Begin<PaneFlag>(m_PaneName,
			&vInOutPaneShown , m_PaneFlag, flags))
		{
#ifdef USE_DECORATIONS_FOR_RESIZE_CHILD_WINDOWS
			auto win = ImGui::GetCurrentWindowRead();
			if (win->Viewport->Idx != 0)
				flags |= ImGuiWindowFlags_NoResize;
			else
				flags =	ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoBringToFrontOnFocus;
#endif

			DrawContent();
		}

		ImGui::End();
	}

	return SourcePane_WidgetId;
}

void ToolPane::DrawDialogsAndPopups(const uint32_t& /*vCurrentFrame*/, const std::string& /*vvUserDatas*/)
{
	/*ImVec2 maxSize = MainFrame::Instance()->m_DisplaySize;
	ImVec2 minSize = maxSize * 0.5f;

	if (ImGuiFileDialog::Instance()->Display("OPEN_LUA_SCRIPT_FILE",
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking,
		minSize, maxSize))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			LuaEngine::Instance()->SetLuaFilePathName(ImGuiFileDialog::Instance()->GetFilePathName());
			CodePane::Instance()->SetCodeFile(ImGuiFileDialog::Instance()->GetFilePathName());
			ProjectFile::Instance()->SetProjectChange();
		}

		ImGuiFileDialog::Instance()->Close();
	}*/
}

int ToolPane::DrawWidgets(const uint32_t& /*vCurrentFrame*/, const int& vWidgetId, const std::string& /*vvUserDatas*/)
{
	return vWidgetId;
}

void ToolPane::DrawContent()
{

}
