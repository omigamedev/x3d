#pragma once

#include "device.h"

#include <memory>

struct DeviceVKState;
class Window;
class Swapchain;

class DeviceVK : public Device
{
    std::unique_ptr<DeviceVKState> m_state;
public:
    DeviceVK(const DeviceFeatures& features, const Window& window);
    ~DeviceVK();
    
    void Flush() override;
    std::shared_ptr<Swapchain> CreateSwapchain() override;
    std::shared_ptr<Buffer> CreateBuffer() override;
    DeviceVKState& State();

protected:
    bool Create(const DeviceFeatures& features, const Window& window);
    bool FindDevice(const DeviceFeatures& features);
};
