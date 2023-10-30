#pragma once
#include "IApplicationImpl.hpp"
#include <vector>

class GLFWApplicationImpl : public IApplicationImpl
{
public:
	bool init() override;
	void terminate() override;
	IWindowImpl* createWindow(int width, int height, const std::string& title) override;
	IWindowImpl* createFullscreenWindow(const std::string& title) override;
	double getTime() override;
};
