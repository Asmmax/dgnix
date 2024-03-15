#include "Application.hpp"
#include "Window.hpp"
#include "IApplicationImpl.hpp"
#include <iostream>

#ifdef _DEBUG

Application::StopWrapper::StopWrapper()
{
    _instance = new Application();
}

Application::StopWrapper::~StopWrapper()
{
    if (_instance) {
        delete _instance;
    }
}

Application* Application::StopWrapper::get() const
{
    return _instance;
}

void Application::StopWrapper::stop()
{
    delete _instance;
    _instance = nullptr;
}

#endif

Application::Application():
    _isValid(false)
{
}

void Application::initGraphics()
{
    if (_impl) {
        _isValid = _impl->init();
    }
}

void Application::clear()
{
    if (_isValid && _impl) {
        _window.reset();
        _impl->terminate();
        _isValid = false;
    }
}

Application::~Application()
{
    clear();
}

Application& Application::getInstance()
{
#ifdef _DEBUG
    return *getStopWrapper().get();
#else
    static Application instance;
    return instance;
#endif //_DEBUG
}

#ifdef _DEBUG
Application::StopWrapper& Application::getStopWrapper()
{
    static Application::StopWrapper wrapper;
    return wrapper;
}
#endif // _DEBUG

Window* Application::getWindow(int width, int height, const std::string& title)
{
    if (!_isValid)
        return nullptr;

    if (!_impl)
        return nullptr;

    assert(!_window);

    auto windowImpl = _impl->createWindow(width, height, title);
    if (!windowImpl) {
        return nullptr;
    }

    _window.reset(new Window(windowImpl));
    return _window.get();
}

double Application::GetTime() const
{
    if (!_isValid || !_impl)
        return 0.0;

    return _impl->getTime();
}
