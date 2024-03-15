#pragma once
#include <string>

class IWindowImpl;

class IApplicationImpl
{
public:
	IApplicationImpl() = default;
	IApplicationImpl(const IApplicationImpl& other) = delete;
	IApplicationImpl(IApplicationImpl&& other) = delete;
	IApplicationImpl& operator=(const IApplicationImpl& other) = delete;
	IApplicationImpl& operator=(IApplicationImpl&& other) = delete;

	virtual ~IApplicationImpl() = default;

	virtual bool init() = 0;
	virtual void terminate() = 0;
	virtual IWindowImpl* createWindow(int width, int height, const std::string& title) = 0;
	virtual double getTime() const = 0;
};
