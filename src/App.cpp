#include "App.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <stdio.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#define LUA_PILOTED_APPNAME "LuaPilotedApp"
#define LUA_PILOTED_APP_VERSION_ID "0.0.1"

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

static void glfw_error_callback(int error, const char* description)
{
	printf("Glfw Error %d: %s\n", error, description);
}

bool App::init(const int& vArgc, const char**& vArgv)
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
	main_win_ptr = glfwCreateWindow(1280, 720, LUA_PILOTED_APPNAME, nullptr, nullptr);
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
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
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

	MainFrame::Instance(mainWindow)->Init();

	// Main loop
	int display_w, display_h;
	ImVec2 pos, size;
	while (!glfwWindowShouldClose(mainWindow))
	{
#ifndef _DEBUG
		if (!LuaEngine::Instance()->IsJoinable()) // for not blocking threading progress bar animation
		{
			glfwWaitEventsTimeout(1.0);
		}
#endif
		glfwPollEvents();

		glfwGetFramebufferSize(mainWindow, &display_w, &display_h);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
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

		MainFrame::Instance()->Display(pos, size);

		LuaEngine::Instance()->FinishIfRequired();

		ImGui::Render();

		glViewport(0, 0, display_w, display_h);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		// For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(mainWindow);
	}

	MainFrame::Instance()->Unit();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	glfwDestroyWindow(mainWindow);
	glfwTerminate();

	return 0;
}

void App::run()
{

}

bool App::unit()
{

}

int App::Run(const std::string& vAppPath)
{
	ZoneScoped;

	printf("-----------\n");
	printf("[[ Lumo Beta %s ]]\n", Lumo_BuildId);

	FileHelper::Instance()->SetAppPath(vAppPath);
	FileHelper::Instance()->SetCurDirectory(FileHelper::Instance()->GetAppPath());

#ifdef _DEBUG
	FileHelper::Instance()->CreateDirectoryIfNotExist("debug");
#endif

	FileHelper::Instance()->CreateDirectoryIfNotExist("plugins");
	FileHelper::Instance()->CreateDirectoryIfNotExist("shaders");
	FileHelper::Instance()->CreateDirectoryIfNotExist("projects");

	m_VulkanWindowPtr = vkApi::VulkanWindow::Create(WIDTH, HEIGHT, PROJECT_NAME " beta", false);
	if (m_VulkanWindowPtr)
	{
		const auto& main_window = m_VulkanWindowPtr->getWindowPtr();
		if (Init(main_window))
		{
			MainLoop(main_window);
			Unit(main_window);
		}

		m_VulkanWindowPtr->Unit();
		m_VulkanWindowPtr.reset();
	}

	return 0;
}

bool App::Init(GLFWwindow* vWindow)
{
	ZoneScoped;

	bool res = false;

	// Setup Vulkan
	if (glfwVulkanSupported())
	{
		InitFilesTracker(
			std::bind(&App::UpdateFiles, this, std::placeholders::_1),
			std::list<std::string>{ "projects", "shaders" });

		// Core
		vkApi::VulkanCore::sVulkanShader = VulkanShader::Create();
		if (vkApi::VulkanCore::sVulkanShader)
		{
			bool _use_RTX = true;

			m_VulkanCorePtr = vkApi::VulkanCore::Create(m_VulkanWindowPtr, "Lumo", 1, "Lumo Engine", 1, true, _use_RTX);
			if (m_VulkanCorePtr)
			{
				// apres la creation du core
				CommonSystem::Instance()->CreateBufferObject(m_VulkanCorePtr);

				m_VulkanImGuiOverlayPtr = vkApi::VulkanImGuiOverlay::Create(
					m_VulkanCorePtr, m_VulkanWindowPtr); // needed for alloc ImGui Textures

				View3DPane::Instance()->SetVulkanImGuiRenderer(m_VulkanImGuiOverlayPtr->GetImGuiRenderer());
				View2DPane::Instance()->SetVulkanImGuiRenderer(m_VulkanImGuiOverlayPtr->GetImGuiRenderer());

				ImGui::CustomStyle::Instance();

				// on charge les plugins
				PluginManager::Instance()->LoadPlugins(m_VulkanCorePtr);

				NodeManager::Instance()->Init(m_VulkanCorePtr);

				// apres les autres, car on charge le fichier projet
				MainFrame::Instance(vWindow)->Init();

#ifdef USE_THUMBNAILS
				ImGuiFileDialog::Instance()->SetCreateThumbnailCallback([this](IGFD_Thumbnail_Info* vThumbnail_Info)
					{
						if (vThumbnail_Info &&
							vThumbnail_Info->isReadyToUpload &&
							vThumbnail_Info->textureFileDatas)
						{
							m_VulkanCorePtr->getDevice().waitIdle();

							std::shared_ptr<FileDialogAsset> resPtr = std::shared_ptr<FileDialogAsset>(new FileDialogAsset,
								[](FileDialogAsset* obj)
								{
									delete obj;
								}
							);

							if (resPtr)
							{
								resPtr->texturePtr = Texture2D::CreateFromMemory(
									m_VulkanCorePtr,
									vThumbnail_Info->textureFileDatas,
									vThumbnail_Info->textureWidth,
									vThumbnail_Info->textureHeight,
									vThumbnail_Info->textureChannels);

								if (resPtr->texturePtr)
								{
									auto imguiRendererPtr = m_VulkanImGuiOverlayPtr->GetImGuiRenderer().getValidShared();
									if (imguiRendererPtr)
									{
										resPtr->descriptorSet = imguiRendererPtr->CreateImGuiTexture(
											(VkSampler)resPtr->texturePtr->m_DescriptorImageInfo.sampler,
											(VkImageView)resPtr->texturePtr->m_DescriptorImageInfo.imageView,
											(VkImageLayout)resPtr->texturePtr->m_DescriptorImageInfo.imageLayout);

										vThumbnail_Info->userDatas = (void*)resPtr.get();

										m_FileDialogAssets.push_back(resPtr);

										vThumbnail_Info->textureID = (ImTextureID)&resPtr->descriptorSet;
									}

									delete[] vThumbnail_Info->textureFileDatas;
									vThumbnail_Info->textureFileDatas = nullptr;

									vThumbnail_Info->isReadyToUpload = false;
									vThumbnail_Info->isReadyToDisplay = true;

									m_VulkanCorePtr->getDevice().waitIdle();
								}
							}
						}
					});
				ImGuiFileDialog::Instance()->SetDestroyThumbnailCallback([this](IGFD_Thumbnail_Info* vThumbnail_Info)
					{
						if (vThumbnail_Info)
						{
							if (vThumbnail_Info->userDatas)
							{
								m_VulkanCorePtr->getDevice().waitIdle();

								auto assetPtr = (FileDialogAsset*)vThumbnail_Info->userDatas;
								if (assetPtr)
								{
									assetPtr->texturePtr.reset();
									assetPtr->descriptorSet = vk::DescriptorSet{};
									auto imguiRendererPtr = m_VulkanImGuiOverlayPtr->GetImGuiRenderer().getValidShared();
									if (imguiRendererPtr)
									{
										imguiRendererPtr->DestroyImGuiTexture(&assetPtr->descriptorSet);
									}
								}

								m_VulkanCorePtr->getDevice().waitIdle();
							}
						}
					});
#endif // USE_THUMBNAILS

				vkprof::vkProfiler::Instance()->Init(
					m_VulkanCorePtr->getPhysicalDevice(),
					m_VulkanCorePtr->getDevice());

				RenderDocController::Instance()->Init();
				
				res = true;
			}
		}
	}

	return res;
}

void App::MainLoop(GLFWwindow* vWindow)
{
	while (!glfwWindowShouldClose(vWindow))
	{
		ZoneScoped;

		RenderDocController::Instance()->StartCaptureIfResquested();

		// maintain active, prevent user change via imgui dialog
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable; // Disable Viewport

		glfwPollEvents();

		// to absolutly do beofre all vk rendering commands
		m_VulkanCorePtr->ResetCommandPools();

		Update(); // to do absolutly beofre imgui rendering

		PrepareImGui(ct::ivec4(0, m_VulkanWindowPtr->getWindowResolution()));

		// Merged Rendering
		bool needResize = false;
		if (BeginRender(needResize))
		{
			m_VulkanImGuiOverlayPtr->render();
			EndRender();
		}

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

#ifdef USE_THUMBNAILS
		vkDeviceWaitIdle((VkDevice)m_VulkanCorePtr->getDevice());
		ImGuiFileDialog::Instance()->ManageGPUThumbnails();
#endif

		// delete imgui nodes now
		// like that => no issue with imgui descriptors because after imgui render and before next node computing
		GraphPane::Instance()->DeleteNodesIfAnys();

		// mainframe post actions
		MainFrame::Instance()->PostRenderingActions();

		++m_CurrentFrame;

		//will pause the view until we move the mouse
		//glfwWaitEvents();

		RenderDocController::Instance()->EndCaptureIfResquested();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// RENDER ////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool App::BeginRender(bool& vNeedResize)
{
	ZoneScoped;

	if (m_VulkanCorePtr->AcquireNextImage(m_VulkanWindowPtr))
	{
		m_VulkanCorePtr->frameBegin();

		auto devicePtr = m_VulkanCorePtr->getFrameworkDevice().getValidShared();
		if (devicePtr)
		{
			auto cmd = m_VulkanCorePtr->getGraphicCommandBuffer();
			devicePtr->BeginDebugLabel(&cmd, "ImGui", IMGUI_RENDERER_DEBUG_COLOR);

			{
				TracyVkZone(m_VulkanCorePtr->getTracyContext(), cmd, "Record Renderer Command buffer");
			}

			m_VulkanCorePtr->beginMainRenderPass();

			return true;
		}
	}
	else // maybe a resize will fix
	{
		vNeedResize = true;
	}

	return false;
}

void App::EndRender()
{
	ZoneScoped;

	m_VulkanCorePtr->endMainRenderPass();

	auto cmd = m_VulkanCorePtr->getGraphicCommandBuffer();
	
	auto devicePtr = m_VulkanCorePtr->getFrameworkDevice().getValidShared();
	if (devicePtr)
	{
		devicePtr->EndDebugLabel(&cmd);
	}

	{
		TracyVkCollect(m_VulkanCorePtr->getTracyContext(), cmd);
	}

	{
		vkprof::vkProfiler::Instance()->Collect(cmd);
	}

	m_VulkanCorePtr->frameEnd();
	m_VulkanCorePtr->Present();
}

void App::PrepareImGui(ct::ivec4 vViewport)
{
	ZoneScoped;

	ImGui::SetPUSHID(125);
	PluginManager::Instance()->ResetImGuiID(125);

	// ImGui Calc juste avant de rendre dnas la swapchain
	m_VulkanImGuiOverlayPtr->begin();
	ImGuizmo::BeginFrame();

	auto io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		const auto viewport = ImGui::GetMainViewport();
		if (viewport)
		{
			const auto pos = viewport->WorkPos;
			const auto size = viewport->WorkSize;
			vViewport.x = (int)pos.x;
			vViewport.y = (int)pos.y;
			vViewport.z = (int)size.x;
			vViewport.w = (int)size.y;
		}
	}
	else
	{
		vViewport.x = 0;
		vViewport.y = 0;
	}

	MainFrame::Instance()->Display(m_CurrentFrame, vViewport);

	NodeManager::Instance()->DisplayDialogsAndPopups(m_CurrentFrame, MainFrame::Instance()->m_DisplaySize, ImGui::GetCurrentContext());

	m_VulkanImGuiOverlayPtr->end();
}

void App::Update()
{
	ZoneScoped;

	m_VulkanCorePtr->GetDeltaTime(m_CurrentFrame);

	CommonSystem::Instance()->UploadBufferObjectIfDirty(m_VulkanCorePtr);

	CheckIfTheseAreSomeFileChanges();

	NodeManager::Instance()->Execute(m_CurrentFrame);
}

void App::IncFrame()
{
	++m_CurrentFrame;
}

vkApi::VulkanWindowPtr App::GetWindowPtr()
{
	return m_VulkanWindowPtr;
}

bool App::Unit(GLFWwindow* vWindow)
{
	ZoneScoped;

	UNUSED(vWindow);

	if (m_VulkanCorePtr)
	{
		vkDeviceWaitIdle((VkDevice)m_VulkanCorePtr->getDevice());
	}

	RenderDocController::Instance()->Unit();

	vkprof::vkProfiler::Instance()->Unit();

	m_FileDialogAssets.clear();

	MainFrame::Instance()->Unit(); // detruit tout les panes, dont les nodes

	ProjectFile::Instance()->Clear();
	NodeManager::Instance()->Unit();

	if (m_VulkanImGuiOverlayPtr)
	{
		m_VulkanImGuiOverlayPtr->Unit();
		m_VulkanImGuiOverlayPtr.reset();
	}

	CommonSystem::Instance()->DestroyBufferObject();

	PluginManager::Instance()->Clear();

	if (vkApi::VulkanCore::sVulkanShader)
	{
		vkApi::VulkanCore::sVulkanShader->Unit();
		vkApi::VulkanCore::sVulkanShader.reset();
	}

	if (m_VulkanCorePtr)
	{
		m_VulkanCorePtr->Unit();
		m_VulkanCorePtr.reset();
	}

	return true;
}

#define SHADER_PATH 0

void App::AddPathToTrack(std::string vPathToTrack, bool vCreateDirectoryIfNotExist)
{
	ZoneScoped;

	if (!vPathToTrack.empty())
	{
		if (vCreateDirectoryIfNotExist)
		{
			FileHelper::Instance()->CreateDirectoryIfNotExist(vPathToTrack);
		}

		if (m_PathsToTrack.find(vPathToTrack) == m_PathsToTrack.end()) // non trouv�
		{
			m_PathsToTrack.emplace(vPathToTrack);
			FileHelper::Instance()->puSearchPaths.push_back(vPathToTrack);
			FilesTrackerSystem::Instance()->addWatch(vPathToTrack);
		}
	}
}

void App::InitFilesTracker(std::function<void(std::set<std::string>)> vChangeFunc, std::list<std::string> vPathsToTrack)
{
	ZoneScoped;

	m_ChangeFunc = vChangeFunc;

	for (auto path : vPathsToTrack)
	{
		AddPathToTrack(path, true);
	}

	FilesTrackerSystem::Instance()->Changes = false;
}

void App::CheckIfTheseAreSomeFileChanges()
{
	ZoneScoped;

	FilesTrackerSystem::Instance()->update();

	if (FilesTrackerSystem::Instance()->Changes)
	{
		m_ChangeFunc(FilesTrackerSystem::Instance()->files);

		FilesTrackerSystem::Instance()->files.clear();

		FilesTrackerSystem::Instance()->Changes = false;
	}
}

void App::UpdateFiles(const std::set<std::string>& vFiles) const
{
	ZoneScoped;

	std::set<std::string> res;

	for (auto file : vFiles)
	{
		if (file.find(".vert") != std::string::npos ||
			file.find(".frag") != std::string::npos ||
			file.find(".tess") != std::string::npos ||
			file.find(".eval") != std::string::npos ||
			file.find(".glsl") != std::string::npos ||
			file.find(".geom") != std::string::npos ||
			file.find(".scen") != std::string::npos ||
			file.find(".blue") != std::string::npos ||
			file.find(".comp") != std::string::npos ||
			file.find(".rgen") != std::string::npos ||
			file.find(".rint") != std::string::npos ||
			file.find(".miss") != std::string::npos ||
			file.find(".ahit") != std::string::npos ||
			file.find(".chit") != std::string::npos)
		{
			ct::replaceString(file, "\\", "/");
			res.emplace(file);
		}

		if (file.find(".lum") != std::string::npos)
		{
		}
	}

	if (!res.empty())
	{
		NodeManager::Instance()->UpdateShaders(res);
	}
}