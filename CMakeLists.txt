cmake_minimum_required(VERSION 3.10)

project(PluginTemplate VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
set(CMAKE_GENERATOR_TOOLSET "v142" CACHE STRING "Toolset version")

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/plugins")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(compile_options
    /std:c++20
    /EHsc
    /MP
    /Zi
    /O2
    /bigobj
)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")
set(BMSDK_DIR "${EXTERNAL_DIR}/BMSDK")
set(RLSDK_DIR "${EXTERNAL_DIR}/RLSDK/RLSDK_w_pch_includes")
set(IMGUI_DIR "${EXTERNAL_DIR}/BakkesmodPluginTemplate/IMGUI")
set(MODUTILS_DIR "${EXTERNAL_DIR}/ModUtils")

set(RLSDK_SRC
    ${RLSDK_DIR}/GameDefines.cpp
    ${RLSDK_DIR}/SDK_HEADERS/Core_classes.cpp
    ${RLSDK_DIR}/SDK_HEADERS/Engine_classes.cpp
    ${RLSDK_DIR}/SDK_HEADERS/Extras.cpp
    ${RLSDK_DIR}/SDK_HEADERS/ProjectX_classes.cpp
    ${RLSDK_DIR}/SDK_HEADERS/TAGame_classes.cpp
)

set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_impl_dx11.cpp
    ${IMGUI_DIR}/imgui_impl_win32.cpp
    ${IMGUI_DIR}/imgui_stdlib.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

set(MODUTILS_SRC
    ${MODUTILS_DIR}/gui/GuiTools.cpp
    ${MODUTILS_DIR}/util/Utils.cpp
    ${MODUTILS_DIR}/wrappers/GFxWrapper.cpp
)

add_link_options(/DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF)

add_compile_options(${compile_options})
add_compile_definitions(UNICODE _UNICODE)

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR
    "This project requires a 64-bit build.\n"
    "Make sure your toolchain is configured to use a 64-bit compiler."
  )
endif()

add_library(${PROJECT_NAME}
    SHARED
    ${IMGUI_SRC}
    ${RLSDK_SRC}
    ${MODUTILS_SRC}
    ${SRC_DIR}/Instances.cpp
    ${SRC_DIR}/PluginTemplate.cpp
    ${SRC_DIR}/SettingsGUI.cpp
    ${SRC_DIR}/GuiBase.cpp
    ${SRC_DIR}/pch.cpp
    ${SRC_DIR}/${PROJECT_NAME}.rc
)

target_precompile_headers(${PROJECT_NAME}
    PRIVATE ${SRC_DIR}/pch.h
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    PRIVATE ${CMAKE_SOURCE_DIR}/external
    PRIVATE ${CMAKE_SOURCE_DIR}/external/BMSDK/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/external/BMSDK/lib/pluginsdk.lib
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)

if(NOT "$ENV{CI}" STREQUAL "true")
    set(INSTALLED_BMSDK_DIR "$ENV{APPDATA}/bakkesmod/bakkesmod/bakkesmodsdk" CACHE PATH "Installed BakkesMod SDK directory")

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND "${INSTALLED_BMSDK_DIR}/bakkesmod-patch.exe" "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMENT "Running bakkesmod-patch.exe after building ${PROJECT_NAME}"
    )
endif()