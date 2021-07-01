#pragma once

#include "swapchain.h"

#include <memory>

struct SwapchainDX12State;
class DeviceDX12;
class Window;

class SwapchainDX12 : public Swapchain
{
    std::shared_ptr<DeviceDX12> m_device;
    std::unique_ptr<SwapchainDX12State> m_state;
public:
    SwapchainDX12(const std::shared_ptr<DeviceDX12>& device, const Window& window);
    ~SwapchainDX12();
    SwapchainDX12State& State();

protected:
    friend class DeviceDX12;
    bool Create(const Window& window);
};


