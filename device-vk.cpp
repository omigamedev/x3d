#include "device-vk.h"
#include "device-vk-state.h"
#include "buffer-vk.h"
#include "swapchain-vk.h"
#include "window.h"
#include "window-state.h"

#include <stdexcept>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

DeviceVK::DeviceVK(const DeviceFeatures& features, const Window& window)
{
    m_state = std::make_unique<DeviceVKState>();
    if (!Create(features, window))
        throw std::runtime_error("Could not create DeviceVK in ctor.");
}

DeviceVK::~DeviceVK()
{
    Flush();
}

bool DeviceVK::Create(const DeviceFeatures& features, const Window& window)
{
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
        m_state->dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

    vk::ApplicationInfo instance_app_info;
    instance_app_info.pApplicationName = "VulkanSample";
    instance_app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    instance_app_info.pEngineName = "X3DEngine";
    instance_app_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    instance_app_info.apiVersion = VK_API_VERSION_1_2;
    
    std::vector<const char*> instance_layers;
    if (features.debug)
    {
        instance_layers.push_back("VK_LAYER_KHRONOS_validation");
    }
    
    std::vector<const char*> instance_extensions;
    instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    instance_extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    if (features.debug)
    {
        instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    vk::InstanceCreateInfo instance_info;
    instance_info.pApplicationInfo = &instance_app_info;
    instance_info.enabledLayerCount = (uint32_t)instance_layers.size();
    instance_info.ppEnabledLayerNames = instance_layers.data();
    instance_info.enabledExtensionCount = (uint32_t)instance_extensions.size();
    instance_info.ppEnabledExtensionNames = instance_extensions.data();
    m_state->instance = vk::createInstanceUnique(instance_info);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(*m_state->instance);

    // Create Surface
    vk::Win32SurfaceCreateInfoKHR surface_info;
    surface_info.hinstance = window.State().hInst;
    surface_info.hwnd = window.State().hWnd;
    m_state->surface = m_state->instance->createWin32SurfaceKHRUnique(surface_info);

    if (!FindDevice(features))
        return false;

    return true;
}

bool DeviceVK::FindDevice(const DeviceFeatures& features)
{
    std::vector<vk::PhysicalDevice> physical_devices = m_state->instance->enumeratePhysicalDevices();
    for (const auto& pd : physical_devices)
    {
        auto pd_props = pd.getProperties();
        auto props = pd.getQueueFamilyProperties();
        for (int family_index = 0; family_index < props.size(); family_index++)
        {
            bool support_graphics = (bool)(props[family_index].queueFlags & vk::QueueFlagBits::eGraphics);
            bool support_present = pd.getSurfaceSupportKHR(family_index, *m_state->surface);
            if (support_graphics && support_present && pd_props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
            {
                std::vector<const char*> device_layers;
                std::vector<const char*> device_extensions;
                device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
                if (features.raytracing)
                {
                    device_extensions.push_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
                    device_extensions.push_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
                    device_extensions.push_back(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
                    device_extensions.push_back(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME);
                    device_extensions.push_back(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
                    device_extensions.push_back(VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME);
                }

                std::array<float, 1> queue_priorities{ 1.f };
                vk::DeviceQueueCreateInfo queue_info;
                queue_info.queueFamilyIndex = family_index;
                queue_info.queueCount = 1;
                queue_info.pQueuePriorities = queue_priorities.data();
                vk::StructureChain device_info{
                    vk::DeviceCreateInfo()
                        .setQueueCreateInfoCount(1)
                        .setPQueueCreateInfos(&queue_info)
                        .setEnabledLayerCount((uint32_t)device_layers.size())
                        .setPpEnabledLayerNames(device_layers.data())
                        .setEnabledExtensionCount((uint32_t)device_extensions.size())
                        .setPpEnabledExtensionNames(device_extensions.data()),
                    vk::StructureChain{
                        vk::PhysicalDeviceFeatures2(),
                        vk::PhysicalDeviceVulkan12Features()
                            .setBufferDeviceAddress(true)
                            .setDescriptorIndexing(true),
                        vk::PhysicalDeviceRayTracingPipelineFeaturesKHR()
                            .setRayTracingPipeline(true),
                    }.get<vk::PhysicalDeviceFeatures2>(),
                };
                m_state->device = pd.createDeviceUnique(device_info.get<vk::DeviceCreateInfo>());
                m_state->physical_device = pd;
                m_state->device_family = family_index;
                return true;
            }
        }
    }
    return false;
}

std::shared_ptr<Swapchain> DeviceVK::CreateSwapchain()
{
    return nullptr;
}

std::shared_ptr<Buffer> DeviceVK::CreateBuffer()
{
    return nullptr;
}

DeviceVKState& DeviceVK::State()
{
    return *m_state;
}

void DeviceVK::Flush()
{
    if (m_state && m_state->device)
        m_state->device->waitIdle();
}
