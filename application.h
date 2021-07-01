#pragma once

#include "device-config.h"

#include <memory>
#include <string>

struct ApplicationState;
class Window;
class Device;
class Swapchain;

struct ApplicationConfig
{
    int window_width;
    int window_height;
    std::wstring window_title;
    DeviceAPI api;
    DeviceFeatures features;
};

class ApplicationBase
{
    std::shared_ptr<Window> m_window;
    std::shared_ptr<Device> m_device;
    std::shared_ptr<Swapchain> m_swapchain;
public:
    int Run();
protected:
    ApplicationBase() = default;
    ApplicationBase(const ApplicationBase&) = delete;
    ApplicationBase& operator=(const ApplicationBase&) = delete;
    virtual void Init(ApplicationConfig& config) = 0;
    virtual void Start() = 0;
    virtual void Update(float dt) = 0;
    virtual void Terminate() = 0;
private:
    std::shared_ptr<Device> CreateDevice(DeviceAPI api, const DeviceFeatures& features, const Window& window) const;
    std::shared_ptr<Window> CreateWnd(int width, int height, const std::wstring& title) const;
};
