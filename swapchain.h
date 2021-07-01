#pragma once

class SwapchainImage
{

};

class Swapchain
{
public:
    virtual SwapchainImage GetNextBuffer() = 0;
};
