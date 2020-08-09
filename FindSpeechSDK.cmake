# Find the win10 SDK path.
if ("$ENV{WIN10_SDK_PATH}$ENV{WIN10_SDK_VERSION}" STREQUAL "" )
  get_filename_component(WIN10_SDK_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Microsoft SDKs\\Windows\\v10.0;InstallationFolder]" ABSOLUTE CACHE)
  get_filename_component(TEMP_WIN10_SDK_VERSION "[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Microsoft SDKs\\Windows\\v10.0;ProductVersion]" ABSOLUTE)
  get_filename_component(WIN10_SDK_VERSION ${TEMP_WIN10_SDK_VERSION} NAME CACHE)
else()
  set (WIN10_SDK_PATH $ENV{WIN10_SDK_PATH})
  set (WIN10_SDK_VERSION $ENV{WIN10_SDK_VERSION})
endif()

if (IS_DIRECTORY "${WIN10_SDK_PATH}/Include/${WIN10_SDK_VERSION}.0")
  set(WIN10_SDK_VERSION "${WIN10_SDK_VERSION}.0")
endif (IS_DIRECTORY "${WIN10_SDK_PATH}/Include/${WIN10_SDK_VERSION}.0")

if(DEFINED ENV{SPEECH_SDK_DIR})
else()
    set(SpeechSDK_ROOT_DIR "${WIN10_SDK_PATH}")
endif()

message(STATUS "Searching Speech API in Windows 10 SDK v${WIN10_SDK_VERSION}")
find_path(SpeechSDK_INCLUDE_DIR 
    NAMES sapi.h
    PATHS ${SpeechSDK_ROOT_DIR}/Include
    PATH_SUFFIXES ${WIN10_SDK_VERSION}/um)

find_library(SpeechSDK_LIBRARY
    NAMES sapi
    PATHS ${SpeechSDK_ROOT_DIR}/Lib
    PATH_SUFFIXES ${WIN10_SDK_VERSION}/um/x64)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SpeechSDK
  FOUND_VAR SpeechSDK_FOUND
  REQUIRED_VARS
    SpeechSDK_LIBRARY
    SpeechSDK_INCLUDE_DIR
)

if(SpeechSDK_FOUND)
  set(SpeechSDK_LIBRARIES ${SpeechSDK_LIBRARY})
  set(SpeechSDK_INCLUDE_DIRS ${SpeechSDK_INCLUDE_DIR})
endif()

mark_as_advanced(
    SpeechSDK_LIBRARIES
    SpeechSDK_INCLUDE_DIRS
)