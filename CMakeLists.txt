cmake_minimum_required(VERSION 3.13)
project(a319-copilot)

#
# Setup X-Plane SDK
#
set(XPLANE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDK/CHeaders/" CACHE STRING PATH)
set(XPLANE_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDK/Libraries/Win" CACHE STRING PATH)

#
# Find OpenGL
#
if(MINGW)
  find_package(OpenGL REQUIRED)
  if(OPENGL_FOUND)
    message("OpenGL found for MinGW target")
  else()
    message("Unable to find OpenGL for MinGW target")
  endif()
endif()
       
add_subdirectory(src)
