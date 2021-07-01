#pragma once

#include "swapchain.h"

#include <memory>

struct SwapchainVKState;
class DeviceVK;

class SwapchainImageVK : public SwapchainImage
{

};

class SwapchainVK : public Swapchain
{
    std::weak_ptr<DeviceVK> m_device;
    std::unique_ptr<SwapchainVKState> m_state;
public:
    SwapchainVK(const std::shared_ptr<DeviceVK>& device);
    ~SwapchainVK();
    SwapchainVKState& State();

protected:
    bool Create();
};


