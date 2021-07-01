#include "swapchain-dx12.h"
#include "device-dx12.h"
#include "device-dx12-state.h"
#include "window.h"
#include "window-state.h"

#include <stdexcept>

struct SwapchainDX12State
{
    ComPtr<IDXGISwapChain4> swapchain;
};

SwapchainDX12::SwapchainDX12(const std::shared_ptr<DeviceDX12>& device, const Window& window)
{
    m_state = std::make_unique<SwapchainDX12State>();
    m_device = device;
    if (!Create(window))
        throw std::runtime_error("Cannot create SwapchainDX12 in ctor");
}

SwapchainDX12::~SwapchainDX12()
{

}

SwapchainDX12State& SwapchainDX12::State()
{
    return *m_state;
}

bool SwapchainDX12::Create(const Window& window)
{
    auto& window_state = window.State();
    auto& device_state = m_device->State();
    ComPtr<IDXGIFactory2> factory;
    if (!SUCCEEDED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory))))
        return false;

    ComPtr<IDXGISwapChain1> swapchain;
    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.Width = window_state.size.cx;
    desc.Height = window_state.size.cy;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.Stereo = false;
    desc.SampleDesc = { 1, 0 };
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 2;
    desc.Scaling = DXGI_SCALING_STRETCH;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    desc.Flags = 0;
    if (SUCCEEDED(factory->CreateSwapChainForHwnd(device_state.queue.Get(), window_state.hWnd, &desc, nullptr, nullptr, &swapchain)))
    {
        swapchain.As(&m_state->swapchain);
        return true;
    }
    return false;
}
