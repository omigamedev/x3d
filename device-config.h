#pragma once

enum class DeviceAPI : unsigned char
{
    Direct3D_12,
    Vulkan,
    Metal,
};

struct DeviceFeatures
{
    bool debug = true;
    bool raytracing = false;
};
