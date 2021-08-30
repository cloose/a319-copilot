cmake_minimum_required(VERSION 3.13)
project(a319-copilot)

#
# Setup X-Plane SDK
#
set(XPLANE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDK/CHeaders/" CACHE STRING PATH)
set(XPLANE_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDK/Libraries/Win" CACHE STRING PATH)

#
# Dear ImGui
#
set(IMGUI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui" CACHE STRING PATH)
set(IMGUI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui" CACHE STRING PATH)

#
# Find OpenGL
#
find_package(OpenGL REQUIRED)
       
add_subdirectory(src)
