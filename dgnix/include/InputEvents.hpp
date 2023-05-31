#pragma once
#include <functional>

namespace InputEvents {

	enum class MouseKey : int
	{
		LEFT_BUTTON,
		RIGHT_BUTTON,
		MIDDLE_BUTTON,
		UNDEFINED
	};

	enum class KeyState : int
	{
		KEY_UP,
		KEY_DOWN,
		UNDEFINED
	};

	using MouseButtonCallback = std::function<void(MouseKey, KeyState)>;
	using MouseButtonWithMoveCallback = std::function<void(double, double, MouseKey, KeyState)>;
	using MouseMoveCallback = std::function<void(double, double)>;
	using MouseScrollCallback = std::function<void(double)>;

}
