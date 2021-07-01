#pragma once

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VK_USE_PLATFORM_WIN32_KHR
#define VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan.hpp>

struct DeviceVKState
{
    vk::DynamicLoader dl;
    vk::UniqueInstance instance;
    vk::UniqueSurfaceKHR surface;
    vk::UniqueDevice device;
    vk::UniqueCommandPool cmdpool;
    vk::UniqueDescriptorPool descrpool;

    uint32_t device_family = 0;
    vk::PhysicalDevice physical_device;
    vk::Queue q;
};
