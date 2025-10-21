#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <bakkesmod/plugin/bakkesmodplugin.h>

// Standard C++ Libraries
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <array>
#include <string_view>
#include <optional>
#include <filesystem>
#include <span>
#include <fstream>

// Windows specific
#include <Windows.h>
#include <Psapi.h>
#include <shellapi.h>
#pragma comment(lib, "Shlwapi.lib")

// Libraries
#include <json/single_include/nlohmann/json.hpp>
#include <BakkesmodPluginTemplate/IMGUI/imgui.h>
#include <BakkesmodPluginTemplate/IMGUI/imgui_internal.h>
#include <RLSDK/RLSDK_w_pch_includes/SdkHeaders.hpp>

// Project specific
#include "logging.h"

extern std::shared_ptr<CVarManagerWrapper> _globalCvarManager; // <-- ADD THIS LINE

namespace fs = std::filesystem;
using json = nlohmann::json;