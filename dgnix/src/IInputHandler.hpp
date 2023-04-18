#pragma once
#include <functional>

class IInputHandler
{
public:
	using MouseButtonCallback = std::function<void(double, double)>;
	using MouseMoveCallback = std::function<void(double, double)>;
	using MouseScrollCallback = std::function<void(double)>;

	virtual ~IInputHandler() {}

	virtual void init() = 0;
	virtual void setMouseRightButtonDownCallback(const MouseButtonCallback& callback) = 0;
	virtual void setMouseRightButtonUpCallback(const MouseButtonCallback& callback) = 0;
	virtual void setMouseLeftButtonDownCallback(const MouseButtonCallback& callback) = 0;
	virtual void setMouseLeftButtonUpCallback(const MouseButtonCallback& callback) = 0;
	virtual void setMouseMoveCallback(const MouseMoveCallback& callback) = 0;
	virtual void setMouseScrollCallback(const MouseScrollCallback& callback) = 0;

	virtual bool isRightMousePressed() = 0;
	virtual bool isLeftMousePressed() = 0;
};