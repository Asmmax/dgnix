#pragma once
#include <string>

class IWindowImpl;

class IApplicationImpl
{
public:
	virtual ~IApplicationImpl() {}

	virtual bool init() = 0;
	virtual void terminate() = 0;
	virtual IWindowImpl* createWindow(int width, int height, const std::string& title) = 0;
};
