#include "device.h"
#include "device-vk.h"

std::shared_ptr<Device> Device::Create(DeviceAPI API, const DeviceFeatures& features, const Window& window)
{
    switch (API)
    {
    case DeviceAPI::Direct3D_12:
        return nullptr;
    case DeviceAPI::Vulkan:
        return std::make_shared<DeviceVK>(features, window);
    case DeviceAPI::Metal:
        return nullptr;
    }
    return nullptr;
}
