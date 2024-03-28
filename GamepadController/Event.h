#pragma once
#include <functional>
#include <variant>
#include <map>
#include <type_traits>

enum class XboxControllerEvent
{
	A,
	B,
	X,
	Y,
	LB,
	RB,
	RT,
	LT,
	Back,
	Start,
	Share,
	LStickClick,
	RStickClick,
	LStick,
	RStick,
	DPadUp,
	DPadDown,
	DPadLeft,
	DPadRight
};

using CallbackNoArgs = std::function<void()>;
using CallbackCoords = std::function<void(float,float)>;
using ControllerCallback = std::variant<CallbackNoArgs, CallbackCoords>;

class EventHandler
{
public:
	
	void register_event(XboxControllerEvent _event, ControllerCallback _callback)
	{
		callbacks[_event] = _callback;
	}

	template<typename... Args>
	void trigger_event(XboxControllerEvent _event, Args... _args)
	{
		if (callbacks.find(_event) != callbacks.end())
		{
			std::visit([&_args...](auto&& callback)
				{
					using func_t = std::decay_t<decltype(callback)>;
					if constexpr (std::is_invocable_v<func_t, Args...>)
					{
						callback(std::forward<Args>(_args)...);
					}
				}, callbacks[_event]);
		}
	}

private:
	std::unordered_map<XboxControllerEvent, ControllerCallback> callbacks;
};