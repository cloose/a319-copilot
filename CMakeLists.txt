cmake_minimum_required(VERSION 3.13)

if(MINGW)
  find_package(OpenGL REQUIRED)
  if(OPENGL_FOUND)
    message("OpenGL found for MinGW target")
  else()
    message("Unable to find OpenGL for MinGW target")
  endif()
endif()
       
add_subdirectory(src)
