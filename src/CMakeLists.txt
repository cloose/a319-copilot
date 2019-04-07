cmake_minimum_required(VERSION 3.13)
project(a319-copilot)

include_directories(${OPENGL_INCLUDE_DIR} "../SDK/CHeaders/XPLM")

find_library(XPLM_LIBRARY XPLM_64 ../SDK/Libraries/Win/)
set(STATIC_LIBRARY_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(SRC utils/cominstancecreationfailed.cpp
        utils/command.cpp
        utils/dataref.cpp
        utils/log.cpp
        utils/logwriter.cpp
        beforestartflow.cpp
        button.cpp
        cockpitpreparationflow.cpp
        copilot.cpp
        flightloopprocessor.cpp
        label.cpp
        mainwindow.cpp
        speechsynthesizer.cpp
        window.cpp
        xplane.cpp)

add_compile_definitions(IBM XPLM200 XPLM210 XPLM300 XPLM301)
add_library(a319-copilot SHARED ${SRC})

target_link_libraries(a319-copilot sapi ${OPENGL_LIBRARIES} ${XPLM_LIBRARY} ${STATIC_LIBRARY_LINKER_FLAGS})
