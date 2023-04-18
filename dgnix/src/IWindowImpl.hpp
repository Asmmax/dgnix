#pragma once
#include <memory>

class IGraphicsContext;
class IInputHandler;
class IViewImpl;

class IWindowImpl
{
public:
	virtual ~IWindowImpl() {}

	virtual void init() = 0;
	virtual void terminate() = 0;
	virtual bool isDone() = 0;
	virtual void handle() = 0;
	virtual IGraphicsContext* getGraphicsContext() = 0;
	virtual IInputHandler* getInputHandler() = 0;

	virtual IViewImpl* createView() = 0;
};
