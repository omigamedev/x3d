#pragma once

#include "device-config.h"

#include <memory>

class Buffer;
class Window;
class Swapchain;

class Device
{
public:
    Device() = default;
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;
    
    virtual void Flush() = 0;
    virtual std::shared_ptr<Swapchain> CreateSwapchain() = 0;
    virtual std::shared_ptr<Buffer> CreateBuffer() = 0;

    static std::shared_ptr<Device> Create(DeviceAPI API, const DeviceFeatures& features, const Window& window);
};
