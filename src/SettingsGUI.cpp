#include "pch.h"
#include "Cvars.hpp"
#include "PluginTemplate.hpp"
#include <ModUtils/gui/GuiTools.hpp>
#include "Instances.hpp"

void PluginTemplate::RenderSettings()
{
	auto enabled_cvar = getCvar(Cvars::enabled);
	if (!enabled_cvar)
		return;

	bool enabled = enabled_cvar.getBoolValue();
	if (ImGui::Checkbox("Enable Plugin", &enabled))
	{
		enabled_cvar.setValue(enabled);
	}

	ImGui::Spacing();

	if (ImGui::Button("Test Plugin"))
	{
		cvarManager->executeCommand(Commands::test.name);
	}
	GUI::ToolTip("Changes your name to \"It's working\" and shows a notification.");
}

void PluginTemplate::RenderWindow()
{
	ImGui::TextUnformatted("This is the main plugin window.");
	ImGui::TextUnformatted("You can add your own UI here.");
}