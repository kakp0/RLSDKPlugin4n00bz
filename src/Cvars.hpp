#pragma once

#define CVAR(name, desc) CvarData("pt_" name, desc) // automatically apply plugin prefix

struct CvarData
{
	const char* name;
	const char* description;
};

namespace Cvars
{
// bools
constexpr CvarData enabled = CVAR("enabled", "Toggle Plugin Template on or off");
} // namespace Cvars

namespace Commands
{
constexpr CvarData test = CVAR("test", "Test command");
} // namespace Commands

#undef CVAR