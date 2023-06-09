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

#include "DataPane.h"
#include <Gui/MainFrame.h>
#include <ctools/cTools.h>
#include <ctools/Logger.h>
#include <Panes/ToolPane.h>
#include <imgui/imgui_internal.h>
#include <Panes/Manager/LayoutManager.h>
#include <Contrib/ImWidgets/ImWidgets.h>
#include <cinttypes> // printf zu

static int GeneratorPaneWidgetId = 0;

///////////////////////////////////////////////////////////////////////////////////
//// OVERRIDES ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

bool DataPane::Init()
{
	return true;
}

void DataPane::Unit()
{

}

int DataPane::DrawPanes(const uint32_t& /*vCurrentFrame*/, const int& vWidgetId, const std::string& /*vvUserDatas*/, PaneFlag& vInOutPaneShown)
{
	GeneratorPaneWidgetId = vWidgetId;

	if (vInOutPaneShown & m_PaneFlag)
	{
		static ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_MenuBar;
		if (ImGui::Begin<PaneFlag>(m_PaneName,
			&vInOutPaneShown , m_PaneFlag, flags))
		{
#ifdef USE_DECORATIONS_FOR_RESIZE_CHILD_WINDOWS
			auto win = ImGui::GetCurrentWindowRead();
			if (win->Viewport->Idx != 0)
				flags |= ImGuiWindowFlags_NoResize;// | ImGuiWindowFlags_NoTitleBar;
			else
				flags = ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_MenuBar;
#endif

		}

		//MainFrame::sAnyWindowsHovered |= ImGui::IsWindowHovered();

		ImGui::End();
	}

	return GeneratorPaneWidgetId;
}

void DataPane::DrawDialogsAndPopups(const uint32_t& /*vCurrentFrame*/, const std::string& /*vvUserDatas*/)
{

}

int DataPane::DrawWidgets(const uint32_t& /*vCurrentFrame*/, const int& vWidgetId, const std::string& /*vvUserDatas*/)
{
	return vWidgetId;
}

std::string DataPane::getXml(const std::string& /*vOffset*/, const std::string& /*vUserDatas*/)
{
	std::string str;

	return str;
}

bool DataPane::setFromXml(tinyxml2::XMLElement* vElem, tinyxml2::XMLElement* vParent, const std::string& /*vUserDatas*/)
{
	// The value of this child identifies the name of this element
	std::string strName;
	std::string strValue;
	std::string strParentName;

	strName = vElem->Value();
	if (vElem->GetText())
		strValue = vElem->GetText();
	if (vParent != nullptr)
		strParentName = vParent->Value();

	return true;
}
