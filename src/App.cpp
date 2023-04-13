#include "App.h"

#include <stdio.h>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <Contrib/FontIcons/CustomFont.cpp>
#include <Contrib/FontIcons/CustomFont2.cpp>
#include <Contrib/FontIcons/CustomFontToolBar.cpp>
#include <Contrib/FontIcons/Roboto_Medium.cpp>

#include <Gui/MainFrame.h>
#include <Headers/Globals.h>
#include <Lua/LuaEngine.h>

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

static void glfw_error_callback(int error, const char* description)
{
	printf("Glfw Error %d: %s\n", error, description);
}

bool App::init(const int& vArgc, char** vArgv)
{
	printf("-----------\n");
	printf("[[ %s Beta %s ]]\n", LUA_PILOTED_APPNAME, LUA_PILOTED_APP_VERSION_ID);

	//SetAppPath(std::string(argv[0]));
	//SetCurDirectory(FileHelper::Instance()->GetAppPath());

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create window with graphics context
	auto main_win_ptr = glfwCreateWindow(1280, 720, LUA_PILOTED_APPNAME, nullptr, nullptr);
	if (!main_win_ptr)
		return false;

	glfwMakeContextCurrent(main_win_ptr);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGL())
	{
		printf("Failed to initialize OpenGL loader!\n");
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Viewport
	io.FontAllowUserScaling = true; // activate zoom feature with ctrl + mousewheel
#ifdef USE_DECORATIONS_FOR_RESIZE_CHILD_WINDOWS
	io.ConfigViewportsNoDecoration = false; // toujours mettre une frame au fenetre enfant
#endif

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(main_win_ptr, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// load memory font file
	auto fonts_ptr = ImGui::GetIO().Fonts;
	fonts_ptr->AddFontFromMemoryCompressedBase85TTF(FONT_ICON_BUFFER_NAME_RM, 15.0f);
	static ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	static const ImWchar icons_ranges[] = { ICON_MIN_NDP, ICON_MAX_NDP, 0 };
	fonts_ptr->AddFontFromMemoryCompressedBase85TTF(FONT_ICON_BUFFER_NAME_NDP, 15.0f, &icons_config, icons_ranges);
	static const ImWchar icons_ranges2[] = { ICON_MIN_NDP2, ICON_MAX_NDP2, 0 };
	fonts_ptr->AddFontFromMemoryCompressedBase85TTF(FONT_ICON_BUFFER_NAME_NDP2, 15.0f, &icons_config, icons_ranges2);
	static const ImWchar icons_ranges3[] = { ICON_MIN_NDPTB, ICON_MAX_NDPTB, 0 };
	fonts_ptr->AddFontFromMemoryCompressedBase85TTF(FONT_ICON_BUFFER_NAME_NDPTB, 15.0f, &icons_config, icons_ranges3);

	return MainFrame::Instance(main_win_ptr)->init();
}

void App::run()
{
	auto main_win_ptr = MainFrame::Instance()->GetGLFWwindow();

	int display_w, display_h;
	ImVec2 pos, size;
	while (!glfwWindowShouldClose(main_win_ptr))
	{
#ifndef _DEBUG
		if (!LuaEngine::Instance()->IsJoinable()) // for not blocking threading progress bar animation
		{
			glfwWaitEventsTimeout(1.0);
		}
#endif

		glfwPollEvents();

		glfwGetFramebufferSize(main_win_ptr, &display_w, &display_h);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			if (viewport)
			{
				pos = viewport->WorkPos;
				size = viewport->WorkSize;
			}
		}
		else
		{
			pos = ImVec2(0, 0);
			size = ImVec2((float)display_w, (float)display_h);
		}

		MainFrame::Instance()->display(pos, size);

		LuaEngine::Instance()->FinishIfRequired();

		ImGui::Render();

		glViewport(0, 0, display_w, display_h);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		// For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(main_win_ptr);
	}
}

bool App::unit()
{
	auto main_win_ptr = MainFrame::Instance()->GetGLFWwindow();

	MainFrame::Instance()->unit();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();

	glfwDestroyWindow(main_win_ptr);
	glfwTerminate();

	return 0;
}
