#include "pch.h"
#include "Cvars.hpp"
#include "PluginTemplate.hpp"
#include <ModUtils/gui/GuiTools.hpp>
#include "Instances.hpp"
#include "HookManager.hpp"

// Static variable to control the visibility of the custom window
static bool showModernWindow = false;

// Function to render the new modern GUI
void RenderModernGUI()
{
	if (!showModernWindow)
		return;

	// --- Custom Styling for Modern Window (Black, Rounded) ---
	// Dark Black Background
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.05f, 1.0f));
	// Button Styling (Blue)
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.8f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.6f, 1.0f));

	// Rounded Corners for frames and buttons
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
	// Rounded Corners for window
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
	// Remove border
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	// Set fixed initial position to center the window
	ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_Once);
	ImGui::SetNextWindowPos(
	    ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));

	if (ImGui::Begin("Modern Custom Panel",
	        &showModernWindow,
	        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("A modern, simple panel.");
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Modern Action", ImVec2(-1, 0)))
		{
			// Action for the modern button
			Instances.SpawnNotification("Modern UI", "The new button was clicked!", 3);
		}

		ImGui::End();
	}

	// --- Pop Custom Styling ---
	ImGui::PopStyleVar(3);   // WindowRounding, FrameRounding, WindowBorderSize
	ImGui::PopStyleColor(4); // WindowBg, Button, ButtonHovered, ButtonActive
}

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
		// Toggle the visibility of the custom modern GUI
		showModernWindow = !showModernWindow;

		// Optional: Keep the original notification/command for testing RLSDK
		// cvarManager->executeCommand(Commands::test.name);
	}

	// Call the function to render the custom window
	RenderModernGUI();

	GUI::ToolTip("Changes your name to \"It's working\" and shows a notification.");

	ImGui::Spacing();
	ImGui::Separator();

	// --- START: Hook Diagnostics GUI ---
	if (ImGui::CollapsingHeader("Hook Diagnostics", ImGuiTreeNodeFlags_None))
	{
		const auto& attempts = Hooks.GetAllHookAttempts();

		// Filter attempts into successful and unsuccessful lists
		std::vector<HookAttemptResult> successful;
		std::vector<HookAttemptResult> unsuccessful;

		for (const auto& attempt : attempts)
		{
			if (attempt.isSuccessful)
			{
				successful.push_back(attempt);
			}
			else
			{
				unsuccessful.push_back(attempt);
			}
		}

		// Successful Hooks Section
		ImGui::Text("Successful Hooks (%zu):", successful.size());
		// Use a child window to create a scrollable box for the list
		// FIX: Replaced ImGuiChildFlags_Border with 'true' for older ImGui compatibility
		ImGui::BeginChild("SuccessfulHooks", ImVec2(0, 150), true);
		if (successful.empty())
		{
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No events successfully hooked.");
		}
		else
		{
			for (const auto& hook : successful)
			{
				// Display the hook type and name in green
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "[%s] %s", hook.TypeToString().c_str(), hook.functionName.c_str());
			}
		}
		ImGui::EndChild();

		// Unsuccessful Hooks Section
		ImGui::Spacing();
		ImGui::Text("Unsuccessful Hooks (%zu):", unsuccessful.size());
		// Use a child window for the unsuccessful list
		// FIX: Replaced ImGuiChildFlags_Border with 'true' for older ImGui compatibility
		ImGui::BeginChild("UnsuccessfulHooks", ImVec2(0, 150), true);
		if (unsuccessful.empty())
		{
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No hook failures recorded.");
		}
		else
		{
			for (const auto& hook : unsuccessful)
			{
				// Display the hook type and name in red
				ImGui::TextColored(
				    ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "[%s] %s (Hover for reason)", hook.TypeToString().c_str(), hook.functionName.c_str());

				// Add a tooltip for the reason
				// FIX: Removed ImGuiHoveredFlags_DelayNormal as it is unsupported in older ImGui versions
				if (!hook.reason.empty() && ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Reason: %s", hook.reason.c_str());
				}
				// FIX: Removed ImGuiHoveredFlags_DelayNormal as it is unsupported in older ImGui versions
				else if (hook.reason.empty() && ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Reason: Unknown failure");
				}
			}
		}
		ImGui::EndChild();
	}
	// --- END: Hook Diagnostics GUI ---
}

void PluginTemplate::RenderWindow()
{
	ImGui::TextUnformatted("This is the main plugin window.");
	ImGui::TextUnformatted("You can add your own UI here.");
}
