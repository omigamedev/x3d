#pragma once

#include "device.h"

#include <memory>

struct DeviceDX12State;
class Window;
class Swapchain;

class DeviceDX12 : public Device
{
    std::unique_ptr<DeviceDX12State> m_state;
public:
    DeviceDX12(const DeviceFeatures& features, const Window& window);
    ~DeviceDX12();

    void Flush() override;
    std::shared_ptr<Swapchain> CreateSwapchain() override;
    std::shared_ptr<Buffer> CreateBuffer() override;
    
    DeviceDX12State& State();

protected:
    bool Create(const DeviceFeatures& features, const Window& window);
    bool FindDevice(const DeviceFeatures& features);
    bool EnableDebugLayer();
    bool SetupDebugMessages();
};
