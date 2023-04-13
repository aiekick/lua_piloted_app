cmake_minimum_required(VERSION 3.1)

set(PROJECT LuaPilotedApp)
set(PROJECT_VER 0.1.0)

enable_language(C CXX)

project(
	${PROJECT} 
	VERSION ${PROJECT_VER} 
	LANGUAGES CXX)

## for group smake targets in the dir cmakeTargets
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmakeTargets")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## some defines for debug mode (before 3rdparty.cmake)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

include(cmake/3rdparty.cmake)
add_definitions(${GLFW_DEFINITIONS})
add_definitions(-DUSE_DECORATIONS_FOR_RESIZE_CHILD_WINDOWS) ## for the resize imgui issue when we have child glfw windows
add_definitions(-DUSE_GLFW3)
add_definitions(-DGLFW3)
add_definitions(-DPROJECT_NAME="${PROJECT}")
add_definitions(-DPROJECT_PATH="${CMAKE_SOURCE_DIR}")
add_definitions(-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)
add_definitions(-DIMGUI_ENABLE_FREETYPE)
add_definitions(-DUSE_IMGUI="${CMAKE_SOURCE_DIR}/3rdparty/imgui/imgui.h")
add_definitions(-DUSE_STD_FILESYSTEM)

if(MSVC)
    # Ignore 4055 for glad
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /IGNORE:4055 /std:c++17")
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DMSVC)
    add_definitions(-DGLFW_EXPOSE_NATIVE_WIN32)
    add_definitions(-DNOMINMAX) ## for avoid issue regarding std::max and std::min
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17 -Wdeprecated-declarations -Wunused-parameter")
endif ()

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX _debug)
endif()

file(GLOB PROJECT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
source_group(main FILES ${PROJECT_MAIN})

file(GLOB_RECURSE PROJECT_CMAKE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.cmake)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/cmake PREFIX cmake FILES ${PROJECT_CMAKE_FILES})

file(GLOB_RECURSE PROJECT_SRC_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX src FILES ${PROJECT_SRC_FILES})

file(GLOB IMGUI_IMPL_SOURCE_FILES 
		${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_glfw.cpp
		${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_glfw.h
		${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_opengl3.cpp
		${CMAKE_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_opengl3.h)
source_group(src\\ImGui_Impl FILES ${IMGUI_IMPL_SOURCE_FILES})

file(GLOB IMGUI_FREETYPE_SOURCE_FILES
		${CMAKE_SOURCE_DIR}/3rdparty/imgui/misc/freetype/imgui_freetype.cpp
		${CMAKE_SOURCE_DIR}/3rdparty/imgui/misc/freetype/imgui_freetype.h)
source_group(src\\ImGui_FreeType FILES ${IMGUI_FREETYPE_SOURCE_FILES})

add_executable(${PROJECT}
	${PROJECT_MAIN}
	${PROJECT_SRC_FILES}
	${PROJECT_CMAKE_FILES}
	${IMGUI_IMPL_SOURCE_FILES}
	${IMGUI_FREETYPE_SOURCE_FILES}
)

target_compile_definitions(${PROJECT} PUBLIC -DPROJECT_NAME=${PROJECT})
target_compile_definitions(${PROJECT} PUBLIC -DCUSTOM_IMGUIFILEDIALOG_CONFIG="${CMAKE_SOURCE_DIR}/src/Gui/CustomImGuiFileDialogConfig.h")
target_compile_definitions(${IMGUIFILEDIALOG_LIBRARIES} PUBLIC -DCUSTOM_IMGUIFILEDIALOG_CONFIG="${CMAKE_SOURCE_DIR}/src/Gui/CustomImGuiFileDialogConfig.h")

include_directories(
	src
	3rdparty
	${SOL2_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
	${GLAD_INCLUDE_DIR}
	${IMGUI_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	${LUA_JIT_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIR}
	${IMGUIFILEDIALOG_INCLUDE_DIR}
)

target_link_libraries(${PROJECT} PRIVATE
	${GLFW_LIBRARIES}
	${GLAD_LIBRARIES}
	${IMGUI_LIBRARIES}
	${OPENGL_LIBRARIES}
	${LUA_JIT_LIBRARIES}
	${FREETYPE_LIBRARIES}
	${IMGUIFILEDIALOG_LIBRARIES}
)


set(FINAL_BIN_DIR ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_INSTALL_PREFIX "${FINAL_BIN_DIR}")

if ("${CMAKE_EXE_LINKER_FLAGS}" STREQUAL "/machine:x64")
	set(ARCH x64)
else()
	set(ARCH x32)
endif()

set(${PROJECT}_Output_Name "${PROJECT}_${CMAKE_SYSTEM_NAME}$<$<CONFIG:Debug>:_Debug>$<$<CONFIG:Release>:_Release>$<$<CONFIG:MinSizeRel>:_MinSizeRel>$<$<CONFIG:RelWithDebInfo>:_RelWithDebInfo>_${ARCH}")
set_target_properties(${PROJECT} PROPERTIES	OUTPUT_NAME "${${PROJECT}_Output_Name}")
set_target_properties(${PROJECT} PROPERTIES	RUNTIME_OUTPUT_DIRECTORY "${FINAL_BIN_DIR}")

if(WIN32)
	if (MINGW)
		set(${PROJECT}_Output_Name "${PROJECT}_Mingw32_${ARCH}")
	elseif (CYGWIN)
		set(${PROJECT}_Output_Name "${PROJECT}_Cygwin_${ARCH}")
	elseif(MSVC)
		set(${PROJECT}_Output_Name "${PROJECT}_Msvc_${ARCH}")
	endif()
	set_target_properties(${PROJECT} PROPERTIES	OUTPUT_NAME "${${PROJECT}_Output_Name}")
elseif(UNIX)
	set(${PROJECT}_Output_Name "${PROJECT}_Linux_${ARCH}")
	set_target_properties(${PROJECT} PROPERTIES	OUTPUT_NAME "${${PROJECT}_Output_Name}")
endif()
