### A319 Co-Pilot plugin ###

include_directories(${OPENGL_INCLUDE_DIR} ${XPLANE_INCLUDE_DIR} "./utils" "./ui")

find_library(XPLM_LIBRARY XPLM_64 ${XPLANE_LIBRARY_DIR})

if(MINGW)
  set(STATIC_LIBRARY_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

set(SRC utils/cominstancecreationfailed.cpp
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
        ui/button.cpp
        ui/label.cpp
        ui/textarea.cpp
        copilot.cpp
        flowpage.cpp
        mainwindow.cpp
        speechsynthesizer.cpp
        welcomepage.cpp
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
        ui/button.h
        ui/buttonclickedevent.h
        ui/color.h
        ui/label.h
        ui/textarea.h
        copilot.h
        flightstate.h
        flowpage.h
        mainwindow.h
        page.h
        speechsynthesizer.h
        welcomepage.h
        window.h)

add_compile_definitions(IBM XPLM200 XPLM210 XPLM300 XPLM301)
add_library(a319-copilot SHARED ${SRC} ${HDR})

target_link_libraries(a319-copilot sapi ${OPENGL_LIBRARIES} ${XPLM_LIBRARY} ${STATIC_LIBRARY_LINKER_FLAGS})

set_property(TARGET a319-copilot PROPERTY CXX_STANDARD 17)
