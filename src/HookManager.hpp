#pragma once
#include "bakkesmod/wrappers/Engine/ActorWrapper.h"
#include "bakkesmod/wrappers/GameWrapper.h"
#include <unordered_set>
#include <vector>
#include "logging.h"
#include "Instances.hpp"

enum class HookType
{
	Pre,
	Post
};

struct HookKey
{
	std::string functionName;
	HookType    type;

	bool operator==(const HookKey& other) const { return functionName == other.functionName && type == other.type; }
};

// --- START: New Struct Definition ---
struct HookAttemptResult
{
	std::string functionName;
	HookType    type;
	bool        isSuccessful;
	std::string reason; // Empty if successful

	std::string TypeToString() const { return type == HookType::Pre ? "Pre" : "Post"; }
};
// --- END: New Struct Definition ---

namespace std
{
template <> struct hash<HookKey>
{
	size_t operator()(const HookKey& k) const { return hash<std::string>()(k.functionName) ^ (hash<int>()(static_cast<int>(k.type)) << 1); }
};
} // namespace std

class HookManager
{
	std::shared_ptr<GameWrapper> m_gameWrapper;
	std::unordered_set<HookKey>  m_hookedEvents;
    std::vector<HookAttemptResult> m_hookAttempts;

public:
	void init(const std::shared_ptr<GameWrapper>& gw) { m_gameWrapper = gw; }

	[[nodiscard]] size_t GetHookCount() const { return m_hookedEvents.size(); }
    
    [[nodiscard]] const std::vector<HookAttemptResult>& GetAllHookAttempts() const { return m_hookAttempts; }

	bool hookEvent(const std::string& eventName, HookType type, const std::function<void(std::string)>& callback)
	{
		HookKey key{eventName, type};
        
        std::string failureReason = "";
        bool success = true;

		if (m_hookedEvents.contains(key))
		{
#ifdef THROW_ON_DOUBLE_HOOK // if this throws in the onLoad function BM will just catch the exception and log it in console. aka no crash :(
			throw std::runtime_error("Already hooked " + std::string(type == HookType::Pre ? "Pre" : "Post") + ": \"" + eventName + "\"");
#else
			LOG("ERROR: Already hooked {}: \"{}\"", type == HookType::Pre ? "Pre" : "Post", eventName);
#endif
			failureReason = "Already hooked (Duplicate)";
			success = false;
		}

        // CHECK 1: Ensure the function actually exists in the RLSDK before hooking.
        if (success && Instances.FindStaticFunction(eventName) == nullptr)
		{
			LOGERROR("Function does not exist! Hooking failed for: \"{}\"", eventName);
            failureReason = "Function does not exist (RLSDK nullptr)";
			success = false;
		}
        
        if (success) {
            switch (type)
            {
            case HookType::Pre:
                m_gameWrapper->HookEvent(eventName, callback);
                break;
            case HookType::Post:
                m_gameWrapper->HookEventPost(eventName, callback);
                break;
            default:
                LOGERROR("Invalid hook type for event: \"{}\"", eventName);
                failureReason = "Invalid HookType";
                success = false;
            }
        }
        
		if (success)
		{
			m_hookedEvents.insert(key);
			LOG("Hooked function {}: \"{}\"", type == HookType::Pre ? "PRE" : "POST", eventName);
		}

        m_hookAttempts.push_back({eventName, type, success, failureReason});
        
        return success;
	}

	bool hookEvent(const std::string& eventName, HookType type, const std::function<void(ActorWrapper, void*, std::string)>& callback)
	{
		HookKey key{eventName, type};

        std::string failureReason = "";
        bool success = true;

		if (m_hookedEvents.contains(key))
		{
			LOGERROR("Already hooked {}: \"{}\"", type == HookType::Pre ? "Pre" : "Post", eventName);
			failureReason = "Already hooked (Duplicate)";
			success = false;
		}

        // CHECK 2: Ensure the function actually exists in the RLSDK before hooking.
        if (success && Instances.FindStaticFunction(eventName) == nullptr)
		{
			LOGERROR("Function does not exist! Hooking failed for: \"{}\"", eventName);
            failureReason = "Function does not exist (RLSDK nullptr)";
			success = false;
		}

        if (success) {
            switch (type)
            {
            case HookType::Pre:
                m_gameWrapper->HookEventWithCaller<ActorWrapper>(eventName, callback);
                break;
            case HookType::Post:
                m_gameWrapper->HookEventWithCallerPost<ActorWrapper>(eventName, callback);
                break;
            default:
                LOGERROR("Invalid hook type for event: \"{}\"", eventName);
                failureReason = "Invalid HookType";
                success = false;
            }
        }

		if (success)
        {
            m_hookedEvents.insert(key);
        }

        m_hookAttempts.push_back({eventName, type, success, failureReason});
        
		return success;
	}

	void unhookEvent(const std::string& eventName, HookType type)
	{
		HookKey key{eventName, type};

		auto it = m_hookedEvents.find(key);
		if (it == m_hookedEvents.end())
		{
			LOGERROR(
			    "Unhooking function {} was unsuccessful (it was never hooked): \"{}\"", type == HookType::Pre ? "Pre" : "Post", eventName);
			return;
		}

		switch (type)
		{
		case HookType::Pre:
			m_gameWrapper->UnhookEvent(eventName);
			break;
		case HookType::Post:
			m_gameWrapper->UnhookEventPost(eventName);
			break;
		default:
			LOGERROR("Invalid hook type for event: \"{}\"", eventName);
			break;
		}

		m_hookedEvents.erase(it);
		LOG("Unhooked function {}: \"{}\"", type == HookType::Pre ? "Pre" : "Post", eventName);
	}

	void unhookAllEvents()
	{
		for (const auto& key : m_hookedEvents)
		{
			switch (key.type)
			{
			case HookType::Pre:
				m_gameWrapper->UnhookEvent(key.functionName);
				break;
			case HookType::Post:
				m_gameWrapper->UnhookEventPost(key.functionName);
				break;
			default:
				LOG("ERROR: Invalid hook type for event: \"{}\"", key.functionName);
				break;
			}
		}

		LOG("Unhooked all events ({} total)", m_hookedEvents.size());
		m_hookedEvents.clear();
	}
};

extern class HookManager Hooks;
