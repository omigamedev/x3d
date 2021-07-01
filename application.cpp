#include "application.h"
#include "window.h"
#include "device-vk.h"
#include "device-dx12.h"

#include <windows.h>
#include <chrono>

int ApplicationBase::Run()
{
    ApplicationConfig config;
    Init(config);

    m_window = CreateWnd(config.window_width, config.window_height, config.window_title);
    m_device = CreateDevice(config.api, config.features, *m_window);
    m_swapchain = m_device->CreateSwapchain();

    Start();

    MSG msg;
    auto start = std::chrono::high_resolution_clock::now();
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        
        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration_cast<std::chrono::duration<float>>(now - start).count();
        start = now;

        Update(dt);
    }

    Terminate();

    m_device = nullptr;
    m_window = nullptr;

    return EXIT_SUCCESS;
}

std::shared_ptr<Device> ApplicationBase::CreateDevice(DeviceAPI api, const DeviceFeatures& features, const Window& window) const
{
    switch (api)
    {
    case DeviceAPI::Direct3D_12:
        return std::make_shared<DeviceDX12>(features, window);
        break;
    case DeviceAPI::Vulkan:
        return std::make_shared<DeviceVK>(features, window);
    case DeviceAPI::Metal:
        break;
    }
    return nullptr;
}

std::shared_ptr<Window> ApplicationBase::CreateWnd(int width, int height, const std::wstring& title) const
{
    return std::make_shared<Window>(width, height, title);
}
