#pragma once
#include <memory>

class IGraphicsContext;
class IInputHandler;
class IViewImpl;

class IWindowImpl
{
public:
	IWindowImpl() = default;
	IWindowImpl(const IWindowImpl& other) = delete;
	IWindowImpl(IWindowImpl&& other) = delete;
	IWindowImpl& operator=(const IWindowImpl& other) = delete;
	IWindowImpl& operator=(IWindowImpl&& other) = delete;

	virtual ~IWindowImpl() = default;

	virtual void init() = 0;
	virtual void terminate() = 0;
	virtual bool isDone() = 0;
	virtual void handle() = 0;
	virtual IGraphicsContext* getGraphicsContext() = 0;
	virtual IInputHandler* getInputHandler() = 0;

	virtual IViewImpl* createView() = 0;
	virtual void setFullscreen(int width, int height, int framerate) = 0;
	virtual int getFramerate() const = 0;
	virtual void setWindowed() = 0;
};
