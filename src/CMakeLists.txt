cmake_minimum_required(VERSION 3.13)
project(a319-copilot)

include_directories(${OPENGL_INCLUDE_DIR} "../SDK/CHeaders/XPLM" "./utils")

find_library(XPLM_LIBRARY XPLM_64 ../SDK/Libraries/Win/)
set(STATIC_LIBRARY_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(SRC utils/cominstancecreationfailed.cpp
        utils/command.cpp
        utils/dataref.cpp
        utils/flightloopprocessor.cpp
        utils/log.cpp
        utils/logwriter.cpp
        flows/beforestartflow.cpp
        flows/cockpitpreparationflow.cpp
        ui/button.cpp
        ui/label.cpp
        copilot.cpp
        mainwindow.cpp
        speechsynthesizer.cpp
        window.cpp
        xplane.cpp)

set(HDR utils/comhelper.h
        utils/cominstancecreationfailed.h
        utils/command.h
        utils/dataref.h
        utils/flightloopprocessor.h
        utils/log.h
        utils/logentry.h
        utils/logwriter.h
        flows/beforestartflow.h
        flows/cockpitpreparationflow.h
        flows/flow.h
        ui/button.h
        ui/buttonclickedevent.h
        ui/label.h
        copilot.h
        mainwindow.h
        speechsynthesizer.h
        window.h)

add_compile_definitions(IBM XPLM200 XPLM210 XPLM300 XPLM301)
add_library(a319-copilot SHARED ${SRC} ${HDR})

target_link_libraries(a319-copilot sapi ${OPENGL_LIBRARIES} ${XPLM_LIBRARY} ${STATIC_LIBRARY_LINKER_FLAGS})
