### A319 Co-Pilot plugin ###

include_directories(${OPENGL_INCLUDE_DIR} ${XPLANE_INCLUDE_DIR} ${IMGUI_INCLUDE_DIR} "./utils" "./ui")

find_library(XPLM_LIBRARY XPLM_64 ${XPLANE_LIBRARY_DIR})

if(MINGW)
  set(STATIC_LIBRARY_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

set(SOURCES
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
    utils/cominstancecreationfailed.cpp
    utils/command.cpp
    utils/dataref.cpp
    utils/flightloopprocessor.cpp
    utils/log.cpp
    utils/logwriter.cpp
    flows/afterstartflow.cpp
    flows/aftertakeoffflow.cpp
    flows/airplane.cpp
    flows/beforestartflow.cpp
    flows/beforetakeoffflow.cpp
    flows/cockpitpreparationflow.cpp
    flows/takeoffflow.cpp
    flows/taxiflow.cpp
    ui/rectangle.cpp
    ui/window.cpp
    copilot.cpp
    flowpage.cpp
    mainwindow.cpp
    speechsynthesizer.cpp
    welcomepage.cpp
    xplane.cpp
)

set(HEADERS 
    utils/comhelper.h
    utils/cominstancecreationfailed.h
    utils/command.h
    utils/dataref.h
    utils/flightloopprocessor.h
    utils/log.h
    utils/logentry.h
    utils/logwriter.h
    flows/afterstartflow.h
    flows/aftertakeoffflow.h
    flows/airplane.h
    flows/beforestartflow.h
    flows/beforetakeoffflow.h
    flows/cockpitpreparationflow.h
    flows/takeoffflow.h
    flows/taxiflow.h
    flows/flow.h
    flows/flowstep.h
    ui/buttonclickedevent.h
    ui/color.h
    ui/rectangle.h
    ui/window.h
    copilot.h
    flightstate.h
    flowpage.h
    mainwindow.h
    page.h
    speechsynthesizer.h
    welcomepage.h
)

add_compile_definitions(IBM XPLM200 XPLM210 XPLM300 XPLM301)
add_library(a319-copilot SHARED ${SOURCES} ${HEADERS})

target_link_libraries(a319-copilot sapi ${OPENGL_LIBRARIES} ${XPLM_LIBRARY} ${STATIC_LIBRARY_LINKER_FLAGS})

set_property(TARGET a319-copilot PROPERTY CXX_STANDARD 17)
