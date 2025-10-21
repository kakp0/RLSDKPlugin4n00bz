#pragma once
#include "GuiBase.h"
#include <bakkesmod/plugin/bakkesmodplugin.h>
#include <bakkesmod/plugin/pluginwindow.h>
#include <bakkesmod/plugin/PluginSettingsWindow.h>

#include "version.h"
#include <ModUtils/util/Utils.hpp>
#include <memory>
#include "Cvars.hpp"

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(
    VERSION_BUILD);

class PluginTemplate : public BakkesMod::Plugin::BakkesModPlugin, public SettingsWindowBase, public PluginWindowBase
{
	void onLoad() override;
	void onUnload() override;

	void initCvars();
	void initCommands();
	void initHooks(); // <--- ADD THIS LINE

	CVarWrapper getCvar(const CvarData& cvar);
	void        registerCommand(const CvarData& cvar, std::function<void(std::vector<std::string>)> callback);

	std::shared_ptr<bool> m_enabled = std::make_shared<bool>(true);

public:
	void RenderSettings() override;
	void RenderWindow() override;
};