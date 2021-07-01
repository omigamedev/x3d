#include "swapchain-vk.h"
#include "device-vk.h"
#include "device-vk-state.h"

#include <vulkan/vulkan.hpp>

struct SwapchainVKState
{
    vk::UniqueSwapchainKHR swapchain;
    std::vector<vk::Image> swapchain_images;
};

SwapchainVK::SwapchainVK(const std::shared_ptr<DeviceVK>& device)
{
    m_state = std::make_unique<SwapchainVKState>();
    m_device = device;
    if (!Create())
        throw std::runtime_error("Cannot create SwapchainVK in ctor");
}

SwapchainVK::~SwapchainVK()
{

}

SwapchainVKState& SwapchainVK::State()
{
    return *m_state;
}

bool SwapchainVK::Create()
{
    if (auto device = m_device.lock())
    {
        auto& state = device->State();
        vk::SurfaceCapabilitiesKHR surface_caps = 
            state.physical_device.getSurfaceCapabilitiesKHR(*state.surface);
        std::vector<vk::SurfaceFormatKHR> swapchain_formats = 
            state.physical_device.getSurfaceFormatsKHR(*state.surface);
        vk::SwapchainCreateInfoKHR swapchain_info;
        swapchain_info.surface = *state.surface;
        swapchain_info.minImageCount = 2;
        swapchain_info.imageFormat = vk::Format::eB8G8R8A8Unorm;
        swapchain_info.imageColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        swapchain_info.imageExtent = surface_caps.currentExtent;
        swapchain_info.imageArrayLayers = 1;
        swapchain_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment | 
            vk::ImageUsageFlagBits::eTransferDst;
        swapchain_info.presentMode = vk::PresentModeKHR::eFifo;
        swapchain_info.clipped = true;
        m_state->swapchain = state.device->createSwapchainKHRUnique(swapchain_info);
        m_state->swapchain_images = state.device->getSwapchainImagesKHR(*m_state->swapchain);
        return true;
    }
    return false;
}
