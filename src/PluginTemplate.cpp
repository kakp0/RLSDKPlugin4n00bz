#include "pch.h"
#include "PluginTemplate.hpp"
#include "bakkesmod/wrappers/cvarwrapper.h"
#include "Cvars.hpp"
#include "Instances.hpp"
#include "HookManager.hpp"

// vvv ADD THIS LINE vvv
HookManager Hooks;

BAKKESMOD_PLUGIN(PluginTemplate, "Plugin Template", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void PluginTemplate::onLoad()
{
	_globalCvarManager = cvarManager;

	Hooks.init(gameWrapper);
	if (!Instances.InitGlobals())
		return;

	initCvars();
	initCommands();
	initHooks();

	// The count now only includes successful (non-duplicate, valid-type) hooks.
	size_t      hookCount       = Hooks.GetHookCount();
	std::string notificationMsg = std::format("RLSDK is connected!\n{} event(s) successfully hooked.", hookCount);

	// Spawn the notification with the hook count
	Instances.SpawnNotification("Plugin Template", notificationMsg, 5);

	LOG("PluginTemplate loaded! :)");
}

void PluginTemplate::onUnload() {}

void PluginTemplate::initCvars()
{
	auto enabled_cvar = cvarManager->registerCvar(Cvars::enabled.name, "1", Cvars::enabled.description, true, true, 0, true, 1);
	enabled_cvar.bindTo(m_enabled);

	enabled_cvar.addOnValueChanged(
	    [this](std::string oldVal, CVarWrapper updatedCvar)
	    {
		    std::string msg = std::format("Plugin turned {}", updatedCvar.getBoolValue() ? "ON" : "OFF");
		    gameWrapper->Execute([this, msg](GameWrapper* gw) { Instances.SpawnNotification("Plugin Template", msg, 3); });
	    });
}

void PluginTemplate::initCommands()
{
	registerCommand(Commands::test,
	    [this](std::vector<std::string> args) { Instances.SpawnNotification("Plugin Template", "RLSDK is connected!", 5); });
}





void PluginTemplate::initHooks()
{
	// Your hooks go here
	// Example hook 1: This will be counted.
	Hooks.hookEvent("Function TAGame.GameEvent_TA.EventMadwatchEnded",
	    HookType::Post,
	    [this](std::string eventName)
	    {
		    std::string notification_msg = std::format("The event '{}' has fired!", eventName);
		    Instances.SpawnNotification("Event Hooked!", notification_msg, 3, true);
	    });

	// Example hook 2: This will be counted.
	Hooks.hookEvent("Function ProjectX.WebRequest_X.HandleHttpRequestComplete", HookType::Pre, [this](std::string eventName) { /* ... */ });

	// Example of a duplicate hook: This will NOT be counted by Hooks.GetHookCount().
	Hooks.hookEvent("Function ProjectX.WebRequest_X.HandleHttpRequestComplete", HookType::Post, [this](std::string eventName) { /* ... */ });
}

CVarWrapper PluginTemplate::getCvar(const CvarData& cvar) { return cvarManager->getCvar(cvar.name); }

void PluginTemplate::registerCommand(const CvarData& cvar, std::function<void(std::vector<std::string>)> callback)
{
	cvarManager->registerNotifier(cvar.name, callback, cvar.description, PERMISSION_ALL);
}