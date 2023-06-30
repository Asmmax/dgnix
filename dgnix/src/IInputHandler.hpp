#pragma once
#include "InputEvents.hpp"

class IInputHandler
{
public:
	virtual ~IInputHandler() {}

	virtual void init() = 0;
	virtual void setMouseButtonCallback(const InputEvents::MouseButtonCallback& callback) = 0;
	virtual void setMouseButtonWithMoveCallback(const InputEvents::MouseButtonWithMoveCallback& callback) = 0;
	virtual void setMouseMoveCallback(const InputEvents::MouseMoveCallback& callback) = 0;
	virtual void setMouseScrollCallback(const InputEvents::MouseScrollCallback& callback) = 0;
	virtual void setKeyCallback(const InputEvents::KeyCallback& callback) = 0;

	virtual void captureMouse() = 0;
	virtual void uncaptureMouse() = 0;
};
